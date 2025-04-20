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