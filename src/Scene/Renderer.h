#pragma once

#include "Core/Defines.h"
#include "OpenGL/Shader.h"
#include "Scene/Scene.h"
#include "OpenGL/UniformBuffer.h"

//A renderPass is the smallest unit of collective render
struct RenderPass {
	const ref<ShaderProgram> program;
	const ref<list<Mesh>> meshList;
};

class Renderer {
public:
	Renderer();

	inline void pushRenderPass(const RenderPass& pass) { renderPassList.emplace_back(pass); }
	void clearScreen();
	void render();

private:
	void renderAPass(const RenderPass& renderPass);

	list<RenderPass> renderPassList;
};
