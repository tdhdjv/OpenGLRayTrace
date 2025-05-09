#pragma once

#include <GLM/glm.hpp>

#include <string>

#include "Core/Defines.h"
#include "OpenGL/Shader.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/VertexArray.h"

#include "Scene/Camera.h"

struct RayTraceMeshData {
	glm::mat4 modelMatrix;
	unsigned bvhNodeOffset;
	unsigned triOffset;
	float _pad1;
	float _pad2;

	RayTraceMeshData(const glm::mat4& modelMatrix, unsigned bvhNodeOffset, unsigned triOffset) : 
	modelMatrix(modelMatrix), bvhNodeOffset(bvhNodeOffset), triOffset(triOffset), _pad1(0), _pad2(0)
	{}
};

struct Texture {
	unsigned rendererID;
	std::string type;
};

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

struct MeshData {
	list<Vertex> vertices;
	list<unsigned> indices;
	list<Texture> textures;
};

class Mesh {
public:
	Mesh(const list<Vertex>& vertices, const list<unsigned>& indices, const list<Texture>& textures);
	Mesh(const MeshData& meshData);
	Mesh(const Mesh&) = delete;
	Mesh(Mesh&& mesh) noexcept;

	inline const list<Vertex>& getVertices() const { return vertices; }
	inline const list<unsigned>& getIndices() const { return indices; }
	inline const glm::mat4& getModelMatrix() const { return modelMatrix; }
	inline const size_t getIndicesCount() const { return indices.size(); }

	inline void bind() const { vao.bind(); }

	void setPosition(const glm::vec3 position);
	void setRotation(const glm::vec3 rotation);
private:
	list<Vertex> vertices;
	list<unsigned> indices;
	list<Texture> textures;

	VertexBuffer vbo;
	IndexBuffer ebo;
	VertexArray vao;

	glm::mat4 modelMatrix;
	glm::vec3 position;
	glm::vec3 rotation;
};