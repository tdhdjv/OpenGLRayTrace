#version 460 core

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image2D image;

layout(location = 0) uniform int frame;
layout(location = 1) uniform vec3 position;
layout(location = 2) uniform vec3 viewDir;
layout(location = 3) uniform float blur;
layout(location = 4) uniform float depthOfField;

ivec2 resolution;
vec3 screenRight;
vec3 screenUp;

#define MAX_BOUNCE 8
#define RAY_COUNT 1
#define SPHERE_COUNT 2

#define TAU 6.283185307179586

int testCount = 0;
int testCount2 = 0;
int testCount3 = 0;

#define DEBUG false

const vec3 sunColor = vec3(1.0);
const float sunFocus = 500;
const float sunIntensity = 10;

struct Triangle {
    vec3 positionA;
    float _offset1;
    vec3 positionB;
    float _offset2;
    vec3 positionC;
    float _offset3;

    vec3 normalA;
    float _offset4;
    vec3 normalB;
    float _offset5;
    vec3 normalC;
    float _offset6;
};

#define TAU 6.283185307179586

float rand(inout uint state) {
    state = state * 747796405 + 2891336453;
    return fract(float(state) * 0.0000023283064);
}

vec2 randNormDist(inout uint state) {
    float theta = TAU * rand(state);
    float rho = sqrt(-2 * log(rand(state)));
    return rho * vec2(cos(theta), sin(theta));
}

vec3 randomDir(inout uint state) {
    return normalize(vec3(randNormDist(state), randNormDist(state).x) * 2.0 - 1.0);
}
const uint Lambertian = 0;
const uint Metallic = 1;
const uint Transparent = 2;
const uint Emissive = 3;
const uint Glossy = 4;

struct Material {
    uint MaterialType;
    vec3 color;
    //smoothness
    float value1;
    //None / index of refaction / reflect probablity
    float value2;
};


struct RayHit {
    bool isHit;
    bool isInside;
    vec3 position;
    vec3 normal;
    float t;
    Material mat;
};

struct Ray {
    vec3 origin;
    vec3 dir;
    vec3 invDir;
    float minT;
    float maxT;
    vec3 energy;
    float inverseCull;
};

float fresnel(float cosine, float refraction_index) {
    // Use Schlick's approximation for reflectance.
    float r0 = (1.0 - refraction_index) / (1.0 + refraction_index);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1.0 - cosine),5);
}

Ray rayBounce(Ray ray, in RayHit hit, inout uint state) {
    vec3 dir;
    vec3 energy = hit.mat.color;
    vec3 randDir = randomDir(state);
    vec3 refl = reflect(ray.dir, hit.normal);

    switch(hit.mat.MaterialType) {
    case Lambertian:
        dir = normalize(randDir + hit.normal);
        break;
    
    case Metallic:
        float w = 1.0 - hit.mat.value1;
        dir = randDir*w + refl;
        break;

    case Transparent:
        float eta;

        if(!hit.isInside) {
            eta = 1.0/hit.mat.value2;
            ray.inverseCull = 1.0;
        }
        else {
            eta = hit.mat.value2;
            ray.inverseCull = 1.0;
        }
        vec3 refractDir = refract(ray.dir, hit.normal, eta);
        float cosine = dot(-ray.dir, hit.normal);
        if(length(refractDir) == 0.0 || fresnel(cosine, eta) > rand(state)) { dir = refl; }
        else { dir = refractDir; }
        dir += randDir * (1.0 - hit.mat.value1);
        break;

    case Glossy:
        vec3 diffuse = normalize(randDir + hit.normal);
        if(rand(state) >= hit.mat.value2)  dir = diffuse;
        else {
            dir = hit.mat.value1*refl + (1.0 - hit.mat.value1) * diffuse;
        }
    }
    
    ray.origin = hit.position;
    ray.dir = dir;
    ray.invDir = 1.0/dir;
    ray.maxT = 1000.0;
    ray.energy *= energy;
    return ray;
}


struct Sphere {
    vec3 position;
    float radius;
    Material mat;
};

float square_length(vec3 vec) {
    RayHit hit = RayHit(false, false, vec3(0.0), vec3(0.0, 1.0, 0.0), 0.0, Material(Lambertian, vec3(0.0), 0.0, 0.0));
    return vec.x*vec.x + vec.y*vec.y + vec.z*vec.z;
}

RayHit intersectTriangle(Ray ray, Triangle tri, Material mat, float inverseCull) {
    RayHit hit = RayHit(false, false, vec3(0.0), vec3(0.0, 1.0, 0.0), 0.0, Material(Lambertian, vec3(0.0), 0.0, 0.0));
    
    vec3 edgeAB = tri.positionB - tri.positionA;
    vec3 edgeAC = tri.positionC - tri.positionA;
    vec3 normalVector = cross(edgeAB, edgeAC);
    vec3 ao = ray.origin - tri.positionA;
    vec3 dao = cross(ao, ray.dir);

    float determinant = -dot(ray.dir, normalVector);
    float invDet = 1.0/determinant;

    float dst = dot(ao, normalVector) * invDet;
    float u = dot(edgeAC, dao) * invDet;
    float v = -dot(edgeAB, dao) * invDet;
    float w = 1.0 - u - v;

    hit.isHit = inverseCull* determinant >= 1E-8 && dst >= ray.minT && u >= 0 && v >= 0 && w >= 0;
    hit.isInside = inverseCull == -1.0;
    hit.position = ray.origin + ray.dir * dst;
    hit.normal = inverseCull*normalize(tri.normalA * w + tri.normalB * u + tri.normalC * v);
    hit.t = dst;
    hit.mat = mat;
    return hit;
}

RayHit intersectSphere(Ray ray, Sphere sphere) {
    RayHit hit = RayHit(false, false, vec3(0.0), vec3(0.0, 1.0, 0.0), 0.0, Material(Lambertian, vec3(0.0), 0.0, 0.0));
    vec3 OP = ray.origin - sphere.position;
    float a = square_length(ray.dir);
    float b_half = dot(ray.dir, OP);
    float c = square_length(OP) - sphere.radius * sphere.radius;

    float determinance = b_half*b_half - a*c;

    if(determinance < 0) return hit;
    float t1 = (-b_half - sqrt(determinance))/a;
    float t2 = (-b_half + sqrt(determinance))/a;

    float t = t1;
    if(t < ray.minT || t > ray.maxT) t = t2;
    if(t < ray.minT || t > ray.maxT) return hit;
    hit.isInside = length(OP)-ray.minT < sphere.radius;
    hit.isHit = true;
    hit.t = t;
    hit.position = t * ray.dir + ray.origin;
    hit.normal = normalize(hit.position - sphere.position);
    if(hit.isInside) hit.normal = -hit.normal;
    hit.mat = sphere.mat;
    return hit;
}
struct BVHNode {
    vec3 minPos;
    uint start;

    vec3 maxPos;
    int triCount;
};


float AABBTest(Ray ray, vec3 minPos, vec3 maxPos) {
    vec3 minToOrigin = minPos - ray.origin;
    vec3 maxToOrigin = maxPos - ray.origin;

    vec3 minTs = minToOrigin*ray.invDir;
    vec3 maxTs = maxToOrigin*ray.invDir;

    vec3 t1 = min(minTs, maxTs);
    vec3 t2 = max(minTs, maxTs);

    float nearT = max(t1.x, max(t1.y, t1.z));
    float farT = min(t2.x, min(t2.y, t2.z));

    bool hit = farT >= nearT && farT > 0.0;
    if(!hit) return -1.0;
    float dst = hit ? nearT > 0 ? nearT : ray.minT : -1.0;
    return dst;
}

struct Model {
    mat4 modelMat;
    uint bvhNodeOffset;
    uint triOffset;

    float value1;
    float value2;

    vec3 color;
    uint materialType;
};

layout(std430, binding = 1) buffer TriData {
    Triangle triangles[];
};

layout(std430, binding = 2) buffer BVHData {
    BVHNode BVHNodes[];
};

layout(std430, binding = 3) buffer ModelData {
    Model modelList[];
};


Sphere[SPHERE_COUNT] spheres;

void instanceObjects() {
    spheres[0] = Sphere(vec3(-1.0,  0.0,  0.0), 0.45, Material(Transparent, vec3(0.9), 1.0, 1.33));
    //spheres[1] = Sphere(vec3( 0.0,  0.0,  5.0), 1.0, Material(Lambertian, vec3(1.0), 1.5, 0.0));
}

Ray generateRay(vec2 uv, vec3 screenRight, vec3 screenUp, uint state) {
    vec3 endPoint = position + depthOfField*viewDir + screenRight * uv.x + screenUp * uv.y;
    float angle = TAU * rand(state);
    vec2 offset = sqrt(rand(state)) * vec2(cos(angle), sin(angle));
    vec3 startPoint = position + (screenRight * offset.x + screenUp * offset.y)*blur/resolution.y;
    vec3 dir = normalize(endPoint-startPoint);
    vec3 invDir = 1.0/dir;
    return Ray(startPoint, dir, invDir, 0.001, 1000.0, vec3(1.0), 1.0);
}

RayHit intersectModelNotBVH(Ray ray) {
    RayHit hit = RayHit(false, false, vec3(0.0), vec3(0.0), 0.0, Material(Lambertian, vec3(0.0), 0.0, 0.0));

    for(int i = 0; i < 12; i++) {
        Triangle tri = triangles[i];

        testCount2++;
        RayHit temp = intersectTriangle(ray, tri, Material(Lambertian, vec3(1.0, 0.0, 0.0), 1.0, 0.95), 1.0);
        
        if(temp.isHit) {
            ray.maxT = temp.t;
            hit = temp;
            testCount3++;
            hit.position = hit.position;
            hit.normal = hit.normal;
        }
    }
    return hit;
}

RayHit intersectModel(Ray ray, uint modelIndex) {
    RayHit hit = RayHit(false, false, vec3(0.0), vec3(0.0), 0.0, Material(Lambertian, vec3(0.0), 0.0, 0.0));


    Model model = modelList[modelIndex];
    uint bvhNodeOffset = model.bvhNodeOffset;
    uint triOffset = model.triOffset;

    mat4 modelMatrix = modelList[modelIndex].modelMat;
    mat4 invModel = inverse(modelMatrix);

    vec3 originalDir = ray.dir;
    vec3 originalOrigin = ray.origin;

    ray.origin = vec3(inverse(modelMatrix) * vec4(ray.origin, 1.0));
    ray.dir = vec3(inverse(modelMatrix) * vec4(ray.dir, 0.0));
    ray.invDir = 1.0/ray.dir;

    float t = AABBTest(ray, BVHNodes[bvhNodeOffset].minPos,  BVHNodes[bvhNodeOffset].maxPos);
    if(ray.maxT > t && t > 0.0) {
        uint stack[64];
        int index = 0;
        stack[index++] = 0;


        while(index > 0) {
            //BVH Check
            BVHNode node = BVHNodes[stack[--index]];
            
            if(node.triCount > 0) {
                //triangle tests
                for(int i = 0; i < node.triCount; i++) {
                    Triangle tri = triangles[i + node.start + triOffset];

                    testCount2++;
                    RayHit temp =  intersectTriangle(ray, tri, Material(model.materialType, model.color, model.value1, model.value2) , ray.inverseCull);
                    
                    if(temp.isHit) {
                        ray.maxT = temp.t;
                        hit = temp;
                        testCount3++;
                        hit.position = originalOrigin + originalDir * temp.t;
                        hit.normal = normalize(vec3(modelMatrix * vec4(hit.normal, 0.0)));
                    }
                }
            }

            else {
                //adding Children
                uint smallNodeIndex = node.start + bvhNodeOffset;
                uint bigNodeIndex   = node.start + bvhNodeOffset + 1;

                BVHNode smallNode = BVHNodes[smallNodeIndex];
                BVHNode bigNode = BVHNodes[bigNodeIndex];

                float smallT = AABBTest(ray, smallNode.minPos, smallNode.maxPos);
                float bigT = AABBTest(ray, bigNode.minPos, bigNode.maxPos);
                
                if(smallT > bigT) {
                    uint temp = smallNodeIndex;
                    smallNodeIndex = bigNodeIndex;
                    bigNodeIndex = temp;

                    float tempT = smallT;
                    smallT = bigT;
                    bigT = tempT;
                }

                testCount += 2;
                if(ray.maxT >= bigT && bigT >= ray.minT) { stack[index++] = bigNodeIndex; }
                if(ray.maxT >= smallT && smallT >= ray.minT) { stack[index++] = smallNodeIndex; }
            }
        }
    }

    return hit;
}

RayHit intersectAll(Ray ray) {
    RayHit hit = RayHit(false, false, vec3(0.0), vec3(0.0), 0.0, Material(Lambertian, vec3(0.0), 0.0, 0.0));
    for(int i = 0; i < SPHERE_COUNT; i++) {
        Sphere sphere = spheres[i];
        RayHit temp = intersectSphere(ray, sphere);
        if(temp.isHit) {
            ray.maxT = temp.t;
            hit = temp;
        }
    }

    
    for(int i = 0; i < modelList.length(); i++) {
        RayHit temp = intersectModel(ray, i);
        if(temp.isHit) { 
            ray.maxT = temp.t;
            hit = temp;
        }
    }
    /*
    RayHit temp = intersectModelNotBVH(ray);
    if(temp.isHit) { 
        ray.maxT = temp.t;
        hit = temp;
    }
    */
    return hit;
}

vec3 getEnvironmentLight(vec3 dir) {

    const vec3 groundColour = vec3(0.35, 0.3, 0.35);
    const vec3 skyColourHorizon = vec3(1, 1, 1);
    const vec3 skyColourZenith = vec3(0.08, 0.37, 0.73);
    

    float skyGradientT = pow(smoothstep(0, 0.4, dir.y), 0.35);
    float groundToSkyT = smoothstep(-0.01, 0, dir.y);
    vec3 skyGradient = skyColourHorizon * (1.0 - skyGradientT) + skyColourZenith * skyGradientT;
    vec3 lightPos = vec3(0.0, 1.0, 0.0);

    float sun = pow(max(0, dot(dir, lightPos)), sunFocus) * sunIntensity;
    // Combine ground, sky, and sun
    float aboveGround = groundToSkyT >= 1.0 ? 1.0: 0.0;
    vec3 composite = ( (1.0 -groundToSkyT) * groundColour + skyGradient * groundToSkyT) + sun * sunColor * aboveGround;
    return composite;
}

vec3 getColor(vec2 uv, uint val) {
    vec3 color = vec3(0.0);
    uint state = val;
    for (int j = 0; j < RAY_COUNT; j++) {
        state += j;
        Ray ray = generateRay(uv, screenRight, screenUp, state);

        for (int i = 0; i < MAX_BOUNCE; i++) {
            RayHit hit = intersectAll(ray);
            if(DEBUG) {
                color += vec3(testCount * 0.01, testCount2 * 0.5, testCount3 * 0.5);
                return color;
            }
            if(!hit.isHit) {
                if(DEBUG) break;
                color += ray.energy*getEnvironmentLight(ray.dir);
                break;
            }

            if(hit.mat.MaterialType == Emissive) {
                if(DEBUG) break;
                color += ray.energy * hit.mat.color;
                break;
            }
            ray = rayBounce(ray, hit, state);
        }
    }
    return color/float(RAY_COUNT);
}

void main() {
    ivec2 texCoord = ivec2(gl_GlobalInvocationID.xy);
    resolution = imageSize(image).xy;
    vec2 uv = vec2(texCoord)/vec2(resolution.y);
    uv -= vec2(0.5*resolution.x/resolution.y, 0.5);
    uv *= 2.0;
    uv.x = -uv.x;

    screenRight = normalize(cross(vec3(0.0, 1.0, 0.0), viewDir)) * depthOfField;
    screenUp = cross(viewDir,screenRight);

    instanceObjects();

    vec3 color = getColor(uv, texCoord.x*texCoord.y*texCoord.x + frame*234109);
    color = max(color, vec3(0.0));
    color = sqrt(color);

    vec3 ogColor = imageLoad(image, texCoord).rgb;

    float weight = 1.0 /(float(frame) + 1.0);
    color = color*weight + ogColor*(1.0 - weight);

    imageStore(image, texCoord, vec4(color, 1.0));
}
