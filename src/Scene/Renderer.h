#pragma once

#include "Core/Defines.h"
#include "OpenGL/Shader.h"
#include "Scene/Scene.h"

class Renderer {
public:
	Renderer();

	void clearScreen();
	//We are making assumptions that shaders are always going to have model/view/projection matrices (Change Later!)
	void renderScene(const ref<Scene> scene, const std::string& mode = "default");
	inline void setShader(ref<ShaderProgram> shaderProgram, const std::string& mode = "default") { shaderList[mode] = shaderProgram; }
	template<typename T>
	void setShaderUniform(const char* uniform, const T& value, const std::string& mode = "default");
private:

	//rendering mode, shader
	map<std::string, ref<ShaderProgram>> shaderList;
};
