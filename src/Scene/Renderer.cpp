#include "Renderer.h"

#include <iostream>

void Renderer::renderScene(const Camera& camera, const Scene& scene, const std::string& mode) {
	if (shaderList.find(mode) == shaderList.end()) {
		std::cout << "There isn't a shader in mode:" << mode << std::endl;
		return;
	}
	ref<ShaderProgram> program = shaderList[mode];

	program->setUniform<glm::mat4>("view", camera.getViewMat());
	for (const ref<Mesh> mesh : scene.getMeshes()) {
		mesh->bind();
		program->setUniform<glm::mat4>("model", mesh->getModelMatrix());
		glDrawElements(GL_TRIANGLES, mesh->getIndicesCount(), GL_UNSIGNED_INT, NULL);
	}
}

/*
template<typename T>
void Renderer::setShaderUniform(const char* uniform, const T& value, const std::string& mode)
{
	if (shaderList.find(mode) != shaderList.end())) {
		ref<ShaderProgram> program = shaderList[mode];
		program->setUniform<T>(value);
	}
	else {
		std::cout << "No Shader in mode called: " + mode << std::endl;
	}
}
*/