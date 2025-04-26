#include "res/shaders/RayTrace/Material.glsl"
#include "res/shaders/RayTrace/Random.glsl"

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
        if(!hit.isInside) eta = 1.0/hit.mat.value1;
        else eta = hit.mat.value1;
        vec3 refractDir = refract(ray.dir, hit.normal, eta);
        float cosine = dot(-ray.dir, hit.normal);
        if(length(refractDir) == 0.0 || fresnel(cosine, eta) > rand(state)) { dir = refl; }
        else { dir = refractDir; }
        dir += randDir * (1.0 - hit.mat.value2);
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

