#pragma once

#include <GLM/glm.hpp>

#include <string>

#include "Core/Defines.h"
#include "OpenGL/Shader.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/VertexArray.h"

#include "Scene/Camera.h"

struct MeshData {
	glm::mat4 modelMatrix;
	unsigned bvhNodeOffset;
	unsigned indicesOffset;
	unsigned verticesOffset;
	float pad1;
};

struct Texture {
	unsigned rendererID;
	std::string type;
};

struct Vertex {
	glm::vec3 position;
	float _pad1;

	glm::vec3 normal;
	float _pad2;
	
	glm::vec2 texCoord;
	float _pad3[2];
};

class Mesh {
public:
	Mesh(list<Vertex> vertices, list<unsigned> indices, list<Texture> textures);

	inline const list<Vertex>& getVertices() const { return vertices; }
	inline const glm::mat4& getModelMatrix() const { return modelMatrix; }
	inline const size_t getIndicesCount() const { return indices.size(); }
	inline list<unsigned>& getIndices() { return indices; }

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