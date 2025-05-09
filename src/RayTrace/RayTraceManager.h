#pragma once

#include "Scene/Scene.h"
#include "OpenGL/ShaderStorageBuffer.h"

class RayTraceManager {
public:
	//just hard code the raytracing shader! MAY FIX LATER(probabily not)
	RayTraceManager(const Scene& scene, unsigned width, unsigned height);
	void rayTrace(const Scene& scene);
private:
	ShaderStorageBuffer triSSBO;
	ShaderStorageBuffer bvhSSBO;
	ShaderStorageBuffer modelSSBO;


	ShaderProgram rayTraceShader;
	ShaderProgram quadShader;

	unsigned width;
	unsigned height;

	unsigned texture;
	VertexArray quadVAO;

	glm::vec3 previousCamPos;
	glm::vec3 previousCamViewDir;
	
	unsigned frame;
};