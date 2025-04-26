#pragma once

#include <GLM/glm.hpp>
#include "Core/Defines.h"
#include "Scene/Camera.h"
#include "Scene/Mesh.h"

#define MAX_DEPTH 16
#define INF std::numeric_limits<float>::infinity()

struct BVHData {
	glm::vec3 minPos;
	int triangleStart;

	glm::vec3 maxPos;
	int triCount;
};

struct Tri {
	glm::vec3 minPos;
	glm::vec3 maxPos;
	glm::vec3 total;

	unsigned index1;
	unsigned index2;
	unsigned index3;
};

struct AABB {
	glm::vec3 minPos = glm::vec3(INF);
	glm::vec3 maxPos = glm::vec3(-INF);
};


class BVHNode {
public:
	BVHNode(Mesh& mesh);
	BVHNode();

	void render(ShaderProgram& program, const Camera& camera, unsigned depth = 0);
	list<BVHData> getBVHData();

	const AABB getAABB() const { return aabb; }
	const ref<BVHNode> getLeftNode() const { return leftNode; }
	const ref<BVHNode> getRightNode() const { return rightNode; }
private:
	void setAABB(const list<Tri>& triangles, unsigned start, unsigned end);
	void split(list<Tri>& triangles, unsigned start, unsigned end, unsigned depth = 0);
	void rearangeIndices(list<unsigned>& indices, const list<Tri>& triangles);

	AABB aabb;

	ref<BVHNode> leftNode;
	ref<BVHNode> rightNode;

	unsigned indexStart;
	unsigned triangleCount;
};
