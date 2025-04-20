#include "Core.h"
#include "Window.h"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include <ASSIMP/Importer.hpp>
#include <iostream>

#include "Core.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/ShaderStorageBuffer.h"
#include "Scene/Camera.h"
#include "Scene/BVH.h"
#include "Scene/Mesh.h"
#include "Scene/Model.h"

static void setVAO(const VertexArray& VAO, const ref<BVHNode>& node, unsigned depth = 16) {

	list<ref<BVHNode>> parents;
	list<ref<BVHNode>> children;

	size_t childrenCount = 1;
	parents.push_back(node);
	for(unsigned i = 0; i < depth; i++) {
		//reserve memory before pushing
		childrenCount *= 2;
		children.reserve(childrenCount);
		if (parents.empty() || parents[0]->getLeftNode() == NULL) break;
		for (ref<BVHNode> parent : parents) {
			children.push_back(parent->getLeftNode());
			children.push_back(parent->getRightNode());
		}
		parents = children;
		children.clear();
	}

	//VertexArray VAO;
	float boxVertices[24] = {
		 0.5,  0.5,  0.5,
		 0.5,  0.5, -0.5,
		 0.5, -0.5,  0.5,
		 0.5, -0.5, -0.5,
		-0.5,  0.5,  0.5,
		-0.5,  0.5, -0.5,
		-0.5, -0.5,  0.5,
		-0.5, -0.5, -0.5,
	};

	unsigned boxIndices[36] = {
		2, 1, 0,
		2, 3, 1,

		3, 5, 1,
		3, 7, 5,

		7, 4, 5,
		7, 6, 4,

		6, 0, 4,
		6, 2, 0,

		4, 0, 1,
		1, 5, 4,

		3, 2, 6,
		6, 7, 3,
	};

	list<float> modelMinMaxPos;

	for (ref<BVHNode> node : parents) {
		modelMinMaxPos.push_back(node->getAABB().minPos.x);
		modelMinMaxPos.push_back(node->getAABB().minPos.y);
		modelMinMaxPos.push_back(node->getAABB().minPos.z);

		modelMinMaxPos.push_back(node->getAABB().maxPos.x);
		modelMinMaxPos.push_back(node->getAABB().maxPos.y);
		modelMinMaxPos.push_back(node->getAABB().maxPos.z);
	}

	VertexBuffer vbo(boxVertices, sizeof(boxVertices));
	VertexBuffer modelVBO(modelMinMaxPos.data(), sizeof(float) * modelMinMaxPos.size());
	IndexBuffer ebo(boxIndices, sizeof(boxIndices));
	VertexBufferLayout layout;

	VAO.bind();
	vbo.bind();
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0));
	GLCall(glEnableVertexAttribArray(0));

	modelVBO.bind();
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(3*sizeof(float))));
	GLCall(glEnableVertexAttribArray(2));
	ebo.bind();
	VAO.unBind();

	VAO.bind();
	GLCall(glVertexAttribDivisor(1, 1));
	GLCall(glVertexAttribDivisor(2, 1));
	VAO.unBind();
}

int main() {
	int WIDTH = 1000;
	int HEIGHT = 800;
	Window window(WIDTH, HEIGHT, "Hello UWU!");

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glEnable(GL_CULL_FACE));

	Camera camera;
	
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
	VertexArray QuadVAO;
	VertexBufferLayout QuadLayout;
	QuadLayout.push<float>(2);

	QuadVAO.bind();
	vertexBuffer.bind();
	indexBuffer.bind();
	QuadVAO.unBind();

	QuadVAO.setAttrib(QuadLayout);

	//quad shader
	ShaderProgram quadProgram;
	quadProgram.attachShader(Shader(GL_VERTEX_SHADER, "res/shaders/quad.vert"));
	quadProgram.attachShader(Shader(GL_FRAGMENT_SHADER, "res/shaders/quad.frag"));
	quadProgram.linkProgram();
	quadProgram.bind();
	quadProgram.setUniform<int>("image", 0);


	//compute shader
	ShaderProgram compute;
	compute.attachShader(Shader(GL_COMPUTE_SHADER, "res/shaders/RayTrace/RayTrace.glsl"));
	compute.linkProgram();

	//boxShader
	ShaderProgram boxShader;
	boxShader.attachShader(Shader(GL_VERTEX_SHADER, "res/shaders/box.vert"));
	boxShader.attachShader(Shader(GL_FRAGMENT_SHADER, "res/shaders/box.frag"));
	boxShader.linkProgram();
	boxShader.bind();
	boxShader.setUniform<glm::vec3>("color", { 0.0, 0.0, 1.0 });
	boxShader.setUniform<glm::mat4>("projection", glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.01f, 100.0f));

	//MeshShader
	ShaderProgram meshShader;
	meshShader.attachShader(Shader(GL_VERTEX_SHADER, "res/shaders/render.vert"));
	meshShader.attachShader(Shader(GL_FRAGMENT_SHADER, "res/shaders/render.frag"));
	meshShader.linkProgram();
	meshShader.bind();
	meshShader.setUniform<glm::mat4>("projection", glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.01f, 100.0f));
	
	unsigned texture;
	GLCall(glGenTextures(1, &texture));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL));
	GLCall(glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F));
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture));


	int frame = 0;
	float dt = 0.0;
	double lastTime = glfwGetTime();

	Model model1("res/models/Bunny.obj");
	Model model2("res/models/Dragon.obj");

	//BVH
	ref<BVHNode> node = makeRef<BVHNode>(*model1.getMeshes()[0]);

	VertexArray nodeVAO;
	setVAO(nodeVAO, node);

	list<SSBOData> dataList1;
	list<Vertex> vertices2 = model1.getMeshes()[0]->getVertices();
	list<unsigned> indices2 = model1.getMeshes()[0]->getIndices();
	list<BVHData> BVHDataList = node->getBVHData();
	list<glm::mat4> modelMatrices = list<glm::mat4>();

	modelMatrices.push_back(glm::translate(glm::mat4(1.0f), {0.0, 0.0, 0.0f}));
	modelMatrices.push_back(glm::translate(glm::mat4(1.0f), { 0.0, 0.0, 5.0f }));

	dataList1.emplace_back(vertices2.data(), 12 * sizeof(float) * vertices2.size());
	ShaderStorageBuffer ssbo1(1, dataList1);
	ssbo1.bind();

	list<SSBOData> dataList2;
	dataList2.emplace_back(indices2.data(), sizeof(unsigned) * indices2.size());
	ShaderStorageBuffer ssbo2(2, dataList2);
	ssbo2.bind();

	list<SSBOData> dataList3;
	dataList3.emplace_back(BVHDataList.data(), (sizeof(float) * 8) * BVHDataList.size());
	ShaderStorageBuffer ssbo3(3, dataList3);
	ssbo3.bind();

	list<SSBOData> dataList4;
	dataList4.emplace_back(modelMatrices.data(), sizeof(float) * 16 * modelMatrices.size());
	ShaderStorageBuffer ssbo4(4, dataList4);
	ssbo4.bind();

	//ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.getNative(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
	int depth = 1;
	float blur = 0.0;
	float depthOfField = 1.0;
	float speed = 1.0f;

	while (!window.shouldClose()) {
		double currentTime = glfwGetTime();
		dt = currentTime - lastTime;
		lastTime = currentTime;
		
		TIME_CL(

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 1.0);

		//Input
		if (window.getKey(GLFW_KEY_W)) {
			camera.setPosition(camera.getPosition() + camera.getFront() * speed * dt);
			frame = 0;
		}

		if (window.getKey(GLFW_KEY_A)) {
			camera.setPosition(camera.getPosition() - camera.getRight() * speed * dt);
			frame = 0;
		}

		if (window.getKey(GLFW_KEY_S)) {
			camera.setPosition(camera.getPosition() - camera.getFront() * speed * dt);
			frame = 0;
		}

		if (window.getKey(GLFW_KEY_D)) {
			camera.setPosition(camera.getPosition() + camera.getRight() * speed * dt);
			frame = 0;
		}

		if (window.getKey(GLFW_KEY_SPACE)) {
			camera.setPosition(camera.getPosition() + glm::vec3(0.0, 1.0, 0.0) * speed * dt);
			frame = 0;
		}

		if (window.getKey(GLFW_KEY_LEFT_SHIFT)) {
			camera.setPosition(camera.getPosition() - glm::vec3(0.0, 1.0, 0.0) * speed * dt);
			frame = 0;
		}

		if (window.getKey(GLFW_KEY_RIGHT)) {
			camera.setYaw(camera.getYaw() - dt);
			frame = 0;
		}

		if (window.getKey(GLFW_KEY_LEFT)) {
			camera.setYaw(camera.getYaw() + dt);
			frame = 0;
		}


		if (window.getKey(GLFW_KEY_UP)) {
			camera.setPitch(camera.getPitch() + dt);
			frame = 0;
		}

		if (window.getKey(GLFW_KEY_DOWN)) {
			camera.setPitch(camera.getPitch() - dt);
			frame = 0;
		}
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//rendering

			//rayTracing
		
		compute.bind();
		compute.setUniform<int>("frame", frame);
		compute.setUniform<glm::vec3>("position", camera.getPosition());
		compute.setUniform <glm::vec3>("viewDir", camera.getViewDir());
		compute.setUniform("blur", blur);
		compute.setUniform("depthOfField", depthOfField);
		frame++;
		glDispatchCompute(WIDTH / 8, HEIGHT / 8, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		
		
		quadProgram.bind();
		QuadVAO.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		//BVH rendering

		/*
		model.render(meshShader, camera);

		boxShader.bind();
		nodeVAO.bind();
		boxShader.setUniformMat4("view", camera.getViewMat());
		boxShader.setUniformVec3("color", {0.0, 1.0, 1.0});
		boxShader.setUniformFloat1("alpha", 1.0);
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, 16384));
		boxShader.setUniformVec3("color", { 0.0, 1.0, 1.0 });
		boxShader.setUniformFloat1("alpha", 0.5);
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, 16384));
		*/

		ImGui::Begin("Parameters");
		ImGui::SliderFloat("Depth of Field", &depthOfField, 0.0, 10.0);
		ImGui::SliderFloat("Blur", &blur, 0.0, 20.0);
		ImGui::SliderInt("Depth", &depth, 0, 12);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		window.pollEvent();
		window.swapBuffer();
			, 80, "Render");
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	std::cin.get();
}