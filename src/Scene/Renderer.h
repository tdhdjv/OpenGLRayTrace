#pragma once

#include "Core/Defines.h"
#include "Renderer/Shader.h"
#include "Scene/Scene.h"

class Renderer {
public:

	//We are making assumptions that shaders are always going to have model/view/projection matrices (Change Later!)
	void renderScene(const Camera& camera, const Scene& scene, const std::string& mode = "default");
	void renderRayTrace(const Camera& camera, const Scene& scene, const ShaderProgram& rayTraceProgram);
	inline void setShader(const std::string& mode, ref<ShaderProgram>&& shaderProgram) { shaderList[mode] = shaderProgram; }
	template<typename T>
	void setShaderUniform(const char* uniform, const T& value, const std::string& mode = "default");
private:

	//rendering mode, shader
	map<std::string, ref<ShaderProgram>> shaderList;
};
