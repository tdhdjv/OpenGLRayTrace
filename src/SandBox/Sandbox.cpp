#include "Core/Application.h"
#include "Core/Defines.h"

extern Application* createApplication();

Application* createApplication() {
	Application* application = new Application();
	ref<Scene> scene = makeRef<Scene>();
	scene->addModel(Model("res/models/Bunny.obj"));
	scene->addModel(Model("res/models/Dragon_8K.obj"));
	scene->addModel(Model("res/models/Cube.obj"));
	(*scene->getMeshes())[0].setPosition(glm::vec3(0.0, 0.0, 1.0));
	(*scene->getMeshes())[1].setPosition(glm::vec3(1.0, 0.0, 1.0));
	application->setCurrentScene(scene);

	ref<ShaderProgram> defaultShader = makeRef<ShaderProgram>();

	defaultShader->attachShader(Shader(GL_VERTEX_SHADER, "res/shaders/render.vert"));
	defaultShader->attachShader(Shader(GL_FRAGMENT_SHADER, "res/shaders/render.frag"));
	defaultShader->linkProgram();
	defaultShader->pushGlobalMat4("projection", &scene->getCamera().getProjectionMat());
	defaultShader->pushGlobalMat4("view", &scene->getCamera().getViewMat());

	ref<list<Mesh>> meshes = scene->getMeshes();
	application->getRenderer().pushRenderPass({ defaultShader, meshes });

	return application;
}