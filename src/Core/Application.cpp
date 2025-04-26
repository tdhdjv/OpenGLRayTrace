#include "Application.h"
#include <iostream>

Application* Application::instance = nullptr;

Application::Application(unsigned width, unsigned height)
	:window(width, height, "Ray Tracing Application") {

	instance = this;

	std::cout << "Hello Program" << std::endl;
	
	ref<ShaderProgram> defaultShader = makeRef<ShaderProgram>();
	defaultShader->attachShader(Shader(GL_VERTEX_SHADER, "res/shaders/render.vert"));
	defaultShader->attachShader(Shader(GL_FRAGMENT_SHADER, "res/shaders/render.frag"));
	defaultShader->linkProgram();

	renderer.setShader(defaultShader);
}

void Application::run() {
	float previousTime = glfwGetTime();
	while (!window.shouldClose()) {
		float currentTime = glfwGetTime();
		float dt = currentTime - previousTime;
		previousTime = currentTime;
		//rendering
		currentScene->update(dt);
		renderer.clearScreen();
		renderer.renderScene(currentScene);
		window.pollEvent();
		window.swapBuffer();
	}
}

void Application::setCurrentScene(ref<Scene> scene) {
	currentScene = scene;
}