#pragma once

#include <GLM/glm.hpp>
#include "Core/Defines.h"
#include "Scene/Camera.h"
#include "Scene/Mesh.h"

#define MAX_DEPTH 12
#define INF std::numeric_limits<float>::infinity()

struct BVHState {
	unsigned start;
	unsigned end;
	unsigned depth;
};

struct BVHNode {
	glm::vec3 minPos;
	unsigned triangleStart;

	glm::vec3 maxPos;
	unsigned triCount;

	BVHNode(glm::vec3 minPos, unsigned triangleStart, glm::vec3 maxPos, unsigned triCount) {
		this->minPos = minPos;
		this->triangleStart = triangleStart;
		this->maxPos = maxPos;
		this->triCount = triCount;
	}
};

struct Tri {
	glm::vec3 positionA;
	float _pad1;
	glm::vec3 positionB;
	float _pad2;
	glm::vec3 positionC;
	float _pad3;

	glm::vec3 normalA;
	float _pad4;
	glm::vec3 normalB;
	float _pad5;
	glm::vec3 normalC;
	float _pad6;

	Tri(glm::vec3 positionA, glm::vec3 positionB, glm::vec3 positionC, glm::vec3 normalA, glm::vec3 normalB, glm::vec3 normalC)
	: positionA(positionA), _pad1(0), positionB(positionB), _pad2(0), positionC(positionC), _pad3(0),
		normalA(normalA), _pad4(0), normalB(normalB), _pad5(6), normalC(normalC), _pad6(0)
	{}
};

struct BVHData {
	list<BVHNode> nodes;
	list<Tri> triangles;
};

struct AABB {
	glm::vec3 minPos = glm::vec3(INF);
	glm::vec3 maxPos = glm::vec3(-INF);
};

BVHData generateBVH(const Mesh& mesh, unsigned depth = MAX_DEPTH);
void split(unsigned nodeIndex, list<BVHNode>& nodes, list<Tri>& triList, unsigned depth);
list<Tri> makeTriList(const list<Vertex>& vertices, const list<unsigned>& indices);

inline glm::vec3 minVec(const glm::vec3& a, const glm::vec3& b) {
	return glm::vec3(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
}

inline glm::vec3 maxVec(const glm::vec3& a, const glm::vec3& b) {
	return glm::vec3(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));
}