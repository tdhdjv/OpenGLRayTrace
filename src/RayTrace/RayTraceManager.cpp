#include "RayTraceManager.h"

#include "RayTrace/BVH.h"

#include <iostream>

RayTraceManager::RayTraceManager(const Scene& scene, unsigned width, unsigned height) : 
	width(width), height(height), frame(0), 
	previousCamPos(glm::vec3(0.0)), previousCamViewDir(glm::vec3(1.0, 0.0, 0.0)) {

	//shader setup
	rayTraceShader.attachShader(Shader(GL_COMPUTE_SHADER, "res/shaders/RayTrace/RayTrace.glsl"));
	rayTraceShader.linkProgram();

	quadShader.attachShader(Shader(GL_VERTEX_SHADER, "res/shaders/quad.vert"));
	quadShader.attachShader(Shader(GL_FRAGMENT_SHADER, "res/shaders/quad.frag"));
	quadShader.linkProgram();

	//quad setup
	GLCall(glGenTextures(1, &texture));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL));

	//binding image in ray tracing
	GLCall(glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F));
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture));

	//quad vao
	float vertices[8] = {
		 1.0,  1.0,
		 1.0, -1.0,
		-1.0, -1.0,
		-1.0,  1.0,
	};

	unsigned indices[6] = {
		2, 1, 0,
		0, 3, 2
	};

	VertexBuffer vertexBuffer(vertices, sizeof(vertices));
	IndexBuffer indexBuffer(indices, sizeof(indices));

	VertexBufferLayout quadLayout;
	quadLayout.push<float>(2);

	quadVAO.bind();
	vertexBuffer.bind();
	indexBuffer.bind();
	quadVAO.unBind();

	quadVAO.setAttrib(quadLayout);

	//image binding
	quadShader.bind();
	quadShader.setInt("image", 0);


	//setting up the SSBO
	unsigned bvhOffset = 0;
	unsigned triOffset = 0;

	list<Tri> triList = list<Tri>();
	list<BVHNode> bvhNode = list<BVHNode>();
	list<RayTraceMeshData> meshDataList = list<RayTraceMeshData>();

	for (const Mesh& mesh : *scene.getMeshes()) {

		BVHData bvhData = generateBVH(mesh);
		list<Tri> triangles = bvhData.triangles;
		list<BVHNode> nodes = bvhData.nodes;

		meshDataList.push_back({ mesh.getModelMatrix(), bvhOffset , triOffset });
		bvhOffset += nodes.size();
		triOffset += triangles.size();

		triList.insert(triList.end(), triangles.begin(), triangles.end());
		bvhNode.insert(bvhNode.end(), nodes.begin(), nodes.end());
	}

	list<SSBOData> triSSBODataList;
	triSSBODataList.emplace_back(triList.data(), 24 * sizeof(float) * triList.size());
	triSSBO = ShaderStorageBuffer(1, triSSBODataList);
	triSSBO.bind();

	list<SSBOData> bvhSSBODataList;
	bvhSSBODataList.emplace_back(bvhNode.data(), 8 * sizeof(float) * bvhNode.size());
	bvhSSBO = ShaderStorageBuffer(2, bvhSSBODataList);
	bvhSSBO.bind();

	list<SSBOData> meshSSBODataList;
	meshSSBODataList.emplace_back(meshDataList.data(), sizeof(float) * 20 * meshDataList.size());
	modelSSBO = ShaderStorageBuffer(3, meshSSBODataList);
	modelSSBO.bind();
}

void RayTraceManager::rayTrace(const Scene& scene) {
	rayTraceShader.bind();
	frame++;

	if (previousCamPos != scene.getCamera().getPosition() || previousCamViewDir != scene.getCamera().getViewDir()) frame = 0;

	rayTraceShader.setInt("frame", frame);

	previousCamPos = scene.getCamera().getPosition();
	previousCamViewDir = scene.getCamera().getViewDir();

	rayTraceShader.setFloat3("position", previousCamPos);
	rayTraceShader.setFloat3("viewDir", previousCamViewDir);
	rayTraceShader.setFloat("blur", 1.0);
	rayTraceShader.setFloat("depthOfField", 1.0);

	GLCall(glDispatchCompute(width / 8, height / 8, 1));
	GLCall(glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT));

	quadShader.bind();
	quadVAO.bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}
