#pragma once

#include "Core.h"

class Window {
public:
	Window(int width, int height, const char* title);
	~Window();

	inline GLFWwindow* getNative() { return nativeWindow; };
	inline bool shouldClose() { return glfwWindowShouldClose(nativeWindow); }
	inline void pollEvent() { glfwPollEvents(); }
	inline void swapBuffer() { 
		glFlush();
		glfwSwapBuffers(nativeWindow);
	}

	inline bool getKey(int key) { return glfwGetKey(nativeWindow, key); }
private:
	int width;
	int height;
	const char* title;

	GLFWwindow* createWindow(int width, int height, const char* title);
	GLFWwindow* nativeWindow;
};