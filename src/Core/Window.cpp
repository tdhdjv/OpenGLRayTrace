#include "Window.h"
#include <iostream>

Window::Window(int width, int height, const char* title) :width(width), height(height), title(title){
	nativeWindow = createWindow(width, height, title);
}

Window::~Window() {
	glfwTerminate();
}

GLFWwindow* Window::createWindow(int width, int height, const char* title) {
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return nullptr;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window;
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	glfwSwapInterval(0);

	if (window == NULL) {
		glfwTerminate();
		std::cout << "Failed to Create Window" << std::endl;
		return nullptr;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	return window;
}