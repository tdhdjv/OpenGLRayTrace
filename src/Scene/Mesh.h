#pragma once

#include <GLM/glm.hpp>

#include <string>

#include "Core/Defines.h"
#include "OpenGL/Shader.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/VertexArray.h"

#include "Scene/Camera.h"

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
	Mesh(const list<Vertex>& vertices, const list<unsigned>& indices, unsigned material, const glm::vec3& color, float value1, float value2);
	Mesh(const MeshData& meshData, unsigned material, const glm::vec3& color, float value1, float value2);
	Mesh(const Mesh&) = delete;
	Mesh(Mesh&& mesh) noexcept;

	inline const list<Vertex>& getVertices() const { return vertices; }
	inline const list<unsigned>& getIndices() const { return indices; }
	inline const size_t getIndicesCount() const { return indices.size(); }

	inline void bind() const { vao.bind(); }

	const glm::mat4 getModelMatrix() const;

	glm::vec3 scale;
	glm::vec3 position;
	glm::vec3 rotation;

	glm::vec3 color;
	unsigned material;

	float value1;
	float value2;
	
private:
	list<Vertex> vertices;
	list<unsigned> indices;

	VertexBuffer vbo;
	IndexBuffer ebo;
	VertexArray vao;
};