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