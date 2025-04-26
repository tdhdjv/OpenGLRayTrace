#pragma once

#include "Scene/Scene.h"

class RayTraceManager {
private:
	ref<ShaderProgram> rayTraceShader;
public:
	RayTraceManager(const Scene& scene, const ref<ShaderProgram> shader);
	void setSSBO();
	void rayTrace();
};