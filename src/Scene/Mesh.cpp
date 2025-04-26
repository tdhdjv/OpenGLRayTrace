#include "Mesh.h"

Mesh::Mesh(list<Vertex> vertices, list<unsigned> indices, list<Texture> textures):
	vertices(vertices), indices(indices), textures(textures),
	vbo(VertexBuffer(vertices.data(), sizeof(Vertex)*vertices.size())), ebo(indices.data(), sizeof(unsigned) * indices.size()),
	modelMatrix(glm::mat4(1.0f)){

	VertexBufferLayout layout;
	layout.push<float>(4);
	layout.push<float>(4);
	layout.push<float>(4);
	vao.bind();
	vbo.bind();
	ebo.bind();
	vao.unBind();
	vao.setAttrib(layout);
}

void Mesh::setPosition(const glm::vec3 position) {
	this->position = position;
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, (float) rotation.length(), rotation);
	modelMatrix = glm::translate(modelMatrix, position);
}

void Mesh::setRotation(const glm::vec3 rotation) {
	this->rotation = rotation;
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, (float)rotation.length(), rotation);
	modelMatrix = glm::translate(modelMatrix, position);
}
