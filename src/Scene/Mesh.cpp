#include "Mesh.h"

Mesh::Mesh(const list<Vertex>& vertices, const list<unsigned>& indices, unsigned material, const glm::vec3& color, float value1, float value):
	vertices(vertices), indices(indices), material(material), color(color), value1(value1), value2(value2),
	vbo(VertexBuffer(vertices.data(), sizeof(Vertex)*vertices.size())), ebo(indices.data(), sizeof(unsigned) * indices.size()),
	scale(glm::vec3(1.0)), position(glm::vec3(0.0)), rotation(glm::vec3(0.0)) {

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

Mesh::Mesh(const MeshData& meshData, unsigned material, const glm::vec3& color, float value1, float value2): Mesh(meshData.vertices, meshData.indices, material, color, value1, value2) {}

Mesh::Mesh(Mesh&& mesh) noexcept:
	vertices(std::move(mesh.vertices)), indices(std::move(mesh.indices)), material(mesh.material), color(mesh.color), value1(mesh.value1), value2(mesh.value2),
	scale(std::move(mesh.scale)), position(std::move(mesh.position)), rotation(std::move(mesh.rotation)),
	vbo(std::move(mesh.vbo)), ebo(std::move(mesh.ebo)), vao(mesh.vao) {}

const glm::mat4 Mesh::getModelMatrix() const {
	glm::mat4 modelMatrix(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	if (rotation.x != 0  || rotation.y != 0 || rotation.z != 0) {
		modelMatrix = glm::rotate(modelMatrix, glm::length(rotation), glm::normalize(rotation));
	}
	modelMatrix = glm::scale(modelMatrix, scale);
	return modelMatrix;
}