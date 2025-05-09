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

void Renderer::render() {
	for (const RenderPass& renderPass : renderPassList) {
		renderAPass(renderPass);
	}
}

void Renderer::renderAPass(const RenderPass& renderPass) {
	//set the uniform per render
	const ref<ShaderProgram> program = renderPass.program;
	const ref<list<Mesh>>meshList = renderPass.meshList;

	program->bind();
	program->updateGlobalUniforms();
	for (const Mesh& mesh : *meshList) {
		mesh.bind();
		//Change Later!!! (Pushing model matrix for every mesh)
		program->setMat4("model", mesh.getModelMatrix());

		unsigned count = mesh.getIndicesCount();
		GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL));
	}
}