#pragma once

#include "Scene/Scene.h"
#include "OpenGL/ShaderStorageBuffer.h"


enum RayTraceMaterialType {
	LAMBERTIAN = 0,
	METALLIC = 1,
	TRANSPARENT = 2,
	EMISSIVE = 3,
	GLOSSY = 4,
};

struct RayTraceMeshData {
	glm::mat4 modelMatrix;
	unsigned bvhNodeOffset;
	unsigned triOffset;

	float value1;
	float value2;

	glm::vec3 color;
	unsigned material;
};

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