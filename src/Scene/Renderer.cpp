#include "Renderer.h"

#include <iostream>

Renderer::Renderer() {
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glEnable(GL_CULL_FACE));
}

void Renderer::clearScreen() {
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glClearColor(0.0, 0.0, 0.0, 1.0));
}

//We are making assumptions that shaders are always going to have model/view/projection matrices (Change Later!)
void Renderer::renderScene(const ref<Scene> scene, const std::string& mode) {
	if (shaderList.find(mode) == shaderList.end()) {
		std::cout << "There isn't a shader in mode:" << mode << std::endl;
		return;
	}
	ref<ShaderProgram> program = shaderList[mode];

	program->bind();
	program->setUniform<glm::mat4>("projection", glm::perspective(glm::radians(45.0f), (float)1000 / (float)800, 0.01f, 100.0f));
	program->setUniform<glm::mat4>("view", scene->getCamera().getViewMat());
	for (const ref<Mesh> mesh : scene->getMeshes()) {
		mesh->bind();
		program->setUniform<glm::mat4>("model", mesh->getModelMatrix());
		unsigned count = mesh->getIndicesCount();
		GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL));
	}
}

template<typename T>
void Renderer::setShaderUniform(const char* uniform, const T& value, const std::string& mode) {
	//if (shaderList.find(mode) != shaderList.end())) {
		ref<ShaderProgram> program = shaderList[mode];
		program->setUniform<T>(value);
	//}
	//else {
		//std::cout << "No Shader in mode called: " + mode << std::endl;
	//}
}