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

