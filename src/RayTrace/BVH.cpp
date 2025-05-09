#include "BVH.h"
#include "Scene/Camera.h"

#include <iostream>
#include <algorithm>
#include <queue>

list<Tri> makeTriList(const list<Vertex>& vertices, const list<unsigned>& indices) {
	list<Tri> triList;
	triList.reserve(indices.size());
	for (unsigned i = 0; i < indices.size(); i += 3) {
		unsigned index1 = indices[i];
		unsigned index2 = indices[i + 1];
		unsigned index3 = indices[i + 2];

		const Vertex& a = vertices[index1];
		const Vertex& b = vertices[index2];
		const Vertex& c = vertices[index3];

		triList.emplace_back(a.position, b.position, c.position, a.normal, b.normal, c.normal);
	}

	return triList;
}

bool boxCompareX(const Tri& tri1, const Tri& tri2) {
	glm::vec3 total1 = tri1.positionA + tri1.positionB + tri1.positionC;
	glm::vec3 total2 = tri2.positionA + tri2.positionB + tri2.positionC;
	return total1.x < total2.x;
}

bool boxCompareY(const Tri& tri1, const Tri& tri2) {
	glm::vec3 total1 = tri1.positionA + tri1.positionB + tri1.positionC;
	glm::vec3 total2 = tri2.positionA + tri2.positionB + tri2.positionC;
	return total1.y < total2.y;
}

bool boxCompareZ(const Tri& tri1, const Tri& tri2) {
	glm::vec3 total1 = tri1.positionA + tri1.positionB + tri1.positionC;
	glm::vec3 total2 = tri2.positionA + tri2.positionB + tri2.positionC;
	return total1.z < total2.z;
}

BVHData generateBVH(const Mesh& mesh, unsigned depth) {
	const list<Vertex>& vertices = mesh.getVertices();
	const list<unsigned>& indices = mesh.getIndices();
	list<Tri> triangles = makeTriList(vertices, indices);
	list<BVHNode> nodes;
	size_t size = 1;
	for (int i = 0; i < depth; i++) { size *= 2; }
	nodes.reserve(size);

	TIME_L(
		split(0, nodes, triangles, depth);
	, "spliting bvh");
	return {nodes, triangles};
}

void split(unsigned nodeIndex, list<BVHNode>& nodes, list<Tri>& triList, unsigned depth) {
	//making queue based bvh
	std::queue<BVHState> bvhQueue;
	
	bvhQueue.emplace(0, triList.size(), depth);
	unsigned bvhNodeTail = 1;

	while (!bvhQueue.empty()) {
		const BVHState& state = bvhQueue.front();

		glm::vec3 minPos = glm::vec3(INF);
		glm::vec3 maxPos = glm::vec3(-INF);
		for (unsigned i = state.start; i < state.end; i++) {
			const Tri& tri = triList[i];
			minPos = minVec(minPos, tri.positionA);
			minPos = minVec(minPos, tri.positionB);
			minPos = minVec(minPos, tri.positionC);

			maxPos = maxVec(maxPos, tri.positionA);
			maxPos = maxVec(maxPos, tri.positionB);
			maxPos = maxVec(maxPos, tri.positionC);
		}

		if (state.depth == 0 || state.end - state.start <= 1) {
			nodes.emplace_back(minPos, state.start, maxPos, state.end - state.start);
			bvhQueue.pop();
			continue;
		}
		else {
			nodes.emplace_back(minPos, bvhNodeTail, maxPos, 0);
		}

		float xLength = maxPos.x - minPos.x;
		float yLength = maxPos.y - minPos.y;
		float zLength = maxPos.z - minPos.z;

		bool(*compare)(const Tri&, const Tri&) = xLength > yLength ? (zLength > xLength ? boxCompareZ : boxCompareX) : (zLength > yLength ? boxCompareZ : boxCompareY);

		std::sort(triList.begin() + state.start, triList.begin() + state.end, compare);

		unsigned mid = state.start + (state.end - state.start) / 2;
		bvhQueue.emplace(state.start, mid, state.depth-1);
		bvhQueue.emplace(mid, state.end, state.depth-1);
		bvhNodeTail += 2;

		bvhQueue.pop();
	}
}