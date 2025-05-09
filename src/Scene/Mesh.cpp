#include "Mesh.h"

Mesh::Mesh(const list<Vertex>& vertices, const list<unsigned>& indices, const list<Texture>& textures):
	vertices(vertices), indices(indices), textures(textures),
	vbo(VertexBuffer(vertices.data(), sizeof(Vertex)*vertices.size())), ebo(indices.data(), sizeof(unsigned) * indices.size()),
	modelMatrix(glm::mat4(1.0f)), position(glm::vec3(0.0)), rotation(glm::vec3(0.0)) {

	VertexBufferLayout layout;
	layout.push<float>(3);
	layout.push<float>(3);
	layout.push<float>(2);
	vao.bind();
	vbo.bind();
	ebo.bind();
	vao.unBind();
	vao.setAttrib(layout);
}

Mesh::Mesh(const MeshData& meshData): Mesh(meshData.vertices, meshData.indices, meshData.textures) {}

Mesh::Mesh(Mesh&& mesh) noexcept:
	vertices(std::move(mesh.vertices)), indices(std::move(mesh.indices)), textures(std::move(mesh.textures)),
	modelMatrix(std::move(mesh.modelMatrix)), position(std::move(mesh.position)), rotation(std::move(mesh.rotation)),
	vbo(std::move(mesh.vbo)), ebo(std::move(mesh.ebo)), vao(mesh.vao) {}

void Mesh::setPosition(const glm::vec3 position) {
	this->position = position;
	modelMatrix = glm::mat4(1.0f);
	if(rotation.length() <= 1E-8) modelMatrix = glm::rotate(modelMatrix, (float) rotation.length(), rotation);
	modelMatrix = glm::translate(modelMatrix, position);
}

void Mesh::setRotation(const glm::vec3 rotation) {
	this->rotation = rotation;
	modelMatrix = glm::mat4(1.0f);
	if (rotation.length() <= 1E-8) modelMatrix = glm::rotate(modelMatrix, (float) rotation.length(), rotation);
	modelMatrix = glm::translate(modelMatrix, position);
}

