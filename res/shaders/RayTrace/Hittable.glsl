#include "res/shaders/RayTrace/Ray.glsl"
#include "res/shaders/RayTrace/Material.glsl"

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
