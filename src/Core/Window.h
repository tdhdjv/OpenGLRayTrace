#pragma once

#include "Core.h"

class Window {
public:
	Window(int width, int height, const char* title);
	~Window();

	inline GLFWwindow* getNative() { return nativeWindow; };
	inline bool shouldClose() { return glfwWindowShouldClose(nativeWindow); }
	inline void pollEvent() { glfwPollEvents(); }
	inline bool getKey(int key) const { return glfwGetKey(nativeWindow, key); }
	inline void swapBuffer() {
		glFlush();
		glfwSwapBuffers(nativeWindow);
	}
private:
	int width;
	int height;
	const char* title;

	GLFWwindow* createWindow(int width, int height, const char* title);
	GLFWwindow* nativeWindow;
};