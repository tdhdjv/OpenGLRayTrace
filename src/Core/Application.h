#pragma once

#include "Core/Window.h"
#include "Scene/Renderer.h"
#include "RayTrace/RayTraceManager.h"

class Application {
public:
	Application(unsigned width = 1000, unsigned height = 800);
	//~Application();
	void run();
	void setCurrentScene(ref<Scene> scene);
	inline const Window& getWindow() const { return window; }
	inline Renderer& getRenderer() { return renderer; }
	static const Application& getInstance() { return *instance; }

private:
	static Application* instance;

	unsigned width;
	unsigned height;

	Window window;
	Renderer renderer;
	ref<Scene> currentScene;
};

Application* createApplication();