#include "Core/Application.h"
#include "Core/Defines.h"

extern Application* createApplication();

Application* createApplication() {
	Application* application = new Application();
	ref<Scene> scene = makeRef<Scene>();
	scene->addModel(Model("res/models/Bunny.obj"));
	application->setCurrentScene(scene);
	return application;
}