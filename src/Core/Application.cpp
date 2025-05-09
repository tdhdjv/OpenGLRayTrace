#include "Application.h"
#include "RayTrace/BVH.h"
#include <iostream>

static void setVAO(const VertexArray& VAO, const list<BVHNode>& nodes) {

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

	for (const BVHNode& node : nodes) { 
		modelMinMaxPos.push_back(node.minPos.x);
		modelMinMaxPos.push_back(node.minPos.y);
		modelMinMaxPos.push_back(node.minPos.z);

		modelMinMaxPos.push_back(node.maxPos.x);
		modelMinMaxPos.push_back(node.maxPos.y);
		modelMinMaxPos.push_back(node.maxPos.z);
	}

	std::cout << modelMinMaxPos.size() << std::endl;

	VertexBuffer vbo(boxVertices, sizeof(boxVertices));
	VertexBuffer modelVBO(modelMinMaxPos.data(), sizeof(float) * modelMinMaxPos.size());
	IndexBuffer ebo(boxIndices, sizeof(boxIndices));
	VertexBufferLayout layout;

	VAO.bind();
	vbo.bind();
	ebo.bind();

	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0));
	GLCall(glEnableVertexAttribArray(0));

	modelVBO.bind();
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(3 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(2));
	VAO.unBind();

	VAO.bind();
	GLCall(glVertexAttribDivisor(1, 1));
	GLCall(glVertexAttribDivisor(2, 1));
	VAO.unBind();
}

Application* Application::instance = nullptr;

Application::Application(unsigned width, unsigned height)
	:window(width, height, "Ray Tracing Application"), width(width), height(height) {

	instance = this;
	std::cout << "Hello Program" << std::endl;
}

void Application::run() {
	double previousTime = glfwGetTime();

	RayTraceManager rayTraceManager(*currentScene, width, height);

	/*
	//temporary
	ShaderProgram boxShader;
	boxShader.attachShader(Shader(GL_VERTEX_SHADER, "res/shaders/box.vert"));
	boxShader.attachShader(Shader(GL_FRAGMENT_SHADER, "res/shaders/box.frag"));
	boxShader.linkProgram();
	const Mesh& bunnyMesh = (*currentScene->getMeshes())[0];
	const VertexArray bvhVAO;
	setVAO(bvhVAO, generateBVH(bunnyMesh).nodes);
	boxShader.bind();
	boxShader.setMat4("projection", currentScene->getCamera().getProjectionMat());
	*/
	
	while (!window.shouldClose()) {
		double currentTime = glfwGetTime();
		double dt = currentTime - previousTime;
		previousTime = currentTime;
		//rendering
		currentScene->update(dt);
		renderer.clearScreen();

		//renderer.render();
		
		//raytracing
		TIME_CL(
			rayTraceManager.rayTrace(*currentScene);
		, 100, "RayTracing");
		/*
		boxShader.bind();
		bvhVAO.bind();
		boxShader.setMat4("view", currentScene->getCamera().getViewMat());
		boxShader.setFloat3("color", { 1.0, 0.0, 0.0 });
		boxShader.setFloat("alpha", 1.0);
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, 16384));
		boxShader.setFloat3("color", { 1.0, 0.0, 0.0 });
		boxShader.setFloat("alpha", 0.5);
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, 16384));
		*/
		
		window.pollEvent();
		window.swapBuffer();
	}
}

void Application::setCurrentScene(ref<Scene> scene) {
	currentScene = scene;
}