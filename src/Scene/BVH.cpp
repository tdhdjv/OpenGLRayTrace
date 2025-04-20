#include "BVH.h"
#include "Camera.h"

#include <iostream>
#include <algorithm>
#include <queue>

static list<Tri> makeTriList(const list<Vertex>& vertices, const list<unsigned>& indices) {
	list<Tri> triList;
	for (unsigned i = 0; i < indices.size(); i += 3) {
		unsigned index1 = indices[i];
		unsigned index2 = indices[i + 1];
		unsigned index3 = indices[i + 2];

		glm::vec3 pos1 = vertices[index1].position;
		glm::vec3 pos2 = vertices[index2].position;
		glm::vec3 pos3 = vertices[index3].position;
		glm::vec3 minPos = glm::min(pos1, glm::min(pos2, pos3));
		glm::vec3 maxPos = glm::max(pos1, glm::max(pos2, pos3));
		glm::vec3 total = pos1 + pos2 + pos3;

		triList.emplace_back(minPos, maxPos, total, index1, index2, index3);
	}

	return triList;
}

static bool boxCompareX(const Tri& tri1, const Tri& tri2) {
	return tri1.total.x < tri2.total.x;
}

static bool boxCompareY(const Tri& tri1, const Tri& tri2) {
	return tri1.total.y < tri2.total.y;
}

static bool boxCompareZ(const Tri& tri1, const Tri& tri2) {
	return tri1.total.z < tri2.total.z;
}

BVHNode::BVHNode(Mesh& mesh) {
	list<Tri> triList = makeTriList(mesh.getVertices(), mesh.getIndices());
	split(triList, 0, triList.size());
	rearangeIndices(mesh.getIndices(), triList);
}

BVHNode::BVHNode():indexStart(0), triangleCount(-1) {}

void BVHNode::setAABB(const list<Tri>& triangles, unsigned start, unsigned end) {
	for (int i = start; i < end; i++) {
		Tri triangle = triangles[i];
		aabb.minPos = glm::min(triangle.minPos, aabb.minPos);
		aabb.maxPos = glm::max(triangle.maxPos, aabb.maxPos);
	}
}

void BVHNode::rearangeIndices(list<unsigned>& indices, const list<Tri>& triangles) {
	list<unsigned> newIndices;
	newIndices.reserve(indices.size());
	for (const Tri& tri: triangles) {
		newIndices.push_back(tri.index1);
		newIndices.push_back(tri.index2);
		newIndices.push_back(tri.index3);
	}
	indices = newIndices;
}

list<BVHData> BVHNode::getBVHData() {
	list<BVHData> result;
	std::queue<ref<BVHNode>> BVHQueue;
	
	unsigned tailPos = 3;
	result.emplace_back(aabb.minPos, 1, aabb.maxPos, -1);

	BVHQueue.emplace(leftNode);
	BVHQueue.emplace(rightNode);

	while (BVHQueue.empty() != true) {
		ref<BVHNode> node = BVHQueue.front();
		BVHQueue.pop();

		if (node->leftNode != NULL && node->rightNode != NULL) {
			result.emplace_back(node->aabb.minPos, tailPos, node->aabb.maxPos, -1);
			BVHQueue.emplace(node->leftNode);
			BVHQueue.emplace(node->rightNode);
			tailPos += 2;
		}
		else {
			result.emplace_back(node->aabb.minPos, node->indexStart, node->aabb.maxPos, node->triangleCount);
		}
	}

	return result;
}

void BVHNode::split(list<Tri>& triangles, unsigned start, unsigned end, unsigned depth) {

	setAABB(triangles, start, end);
	indexStart = start * 3;
	triangleCount = end - start;

	if (depth >= MAX_DEPTH || end - start <= 1) {
		return;
	}

	leftNode = makeRef<BVHNode>();
	rightNode = makeRef<BVHNode>();
	
	glm::vec3 dist = aabb.maxPos - aabb.minPos;
	bool(*compare)(const Tri&, const Tri&) = (dist.x > dist.y) ? (dist.z > dist.x) ? boxCompareZ : boxCompareX : (dist.z > dist.y) ? boxCompareZ : boxCompareY;


	std::sort(triangles.begin() + start, triangles.begin() + end, compare);
	unsigned mid = start + (end-start)/2;

	leftNode->split(triangles, start, mid, depth + 1);
	rightNode->split(triangles, mid, end, depth + 1);
}

void BVHNode::render(ShaderProgram& program, const Camera& camera, unsigned depth) {
	list<ref<BVHNode>> parents;
	list<ref<BVHNode>> children;

	size_t childrenCount = 1;
	parents.push_back(leftNode);
	parents.push_back(rightNode);
	for(unsigned i = 0; i < depth-1; i++) {
		//reserve memory before pushing
		childrenCount *= 2;
		children.reserve(childrenCount);
		if (parents.empty() || parents[0]->leftNode == NULL) break;
		for (ref<BVHNode> parent : parents) {
			children.push_back(parent->leftNode);
			children.push_back(parent->rightNode);
		}
		parents = children;
		children.clear();
	}

	program.bind();


	list<float> modelMinMaxPos;

	for (ref<BVHNode> node : parents) {
		modelMinMaxPos.push_back(node->aabb.minPos.x);
		modelMinMaxPos.push_back(node->aabb.minPos.y);
		modelMinMaxPos.push_back(node->aabb.minPos.z);

		modelMinMaxPos.push_back(node->aabb.maxPos.x);
		modelMinMaxPos.push_back(node->aabb.maxPos.y);
		modelMinMaxPos.push_back(node->aabb.maxPos.z);
	}
}