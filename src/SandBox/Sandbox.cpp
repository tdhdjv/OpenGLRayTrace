#include "Core/Application.h"
#include "Core/Defines.h"
#include "RayTrace/RayTraceManager.h"

extern Application* createApplication();

Application* createApplication() {
	Application* application = new Application();
	ref<Scene> scene = makeRef<Scene>();
	scene->addModel(Model("res/models/Bunny.obj"), METALLIC, glm::vec3(1.0, 1.0, 0.0), 0.95, 1.1);
	scene->addModel(Model("res/models/Dragon_8K.obj"), GLOSSY, glm::vec3(0.0, 1.0, 0.0), 0.95, 0.15);
	scene->addModel(Model("res/models/quad.obj"), LAMBERTIAN, glm::vec3(1.0, 0.0, 0.0), 1.0, 0.0);
	scene->addModel(Model("res/models/quad.obj"), LAMBERTIAN, glm::vec3(0.0, 1.0, 0.0), 1.0, 0.0);
	scene->addModel(Model("res/models/quad.obj"), LAMBERTIAN, glm::vec3(0.0, 0.0, 1.0), 0.0, 0.0);
	scene->addModel(Model("res/models/quad.obj"), LAMBERTIAN, glm::vec3(0.2, 0.2, 0.2), 0.0, 0.0);
	scene->addModel(Model("res/models/quad.obj"), METALLIC, glm::vec3(1.0, 0.1, 1.0), 1.0, 0.1);
	scene->addModel(Model("res/models/quad.obj"), METALLIC, glm::vec3(0.1, 1.0, 1.0), 1.0, 0.1);
	scene->addModel(Model("res/models/quad.obj"), EMISSIVE, glm::vec3(100.0), 0.0, 0.0);

	//position
	(*scene->getMeshes())[0].position = glm::vec3(0.0, -0.5, 0.0);
	(*scene->getMeshes())[1].position = glm::vec3(1.0, 0.0, 0.0);

	(*scene->getMeshes())[2].position = glm::vec3(0.0, 0, 1.5);
	(*scene->getMeshes())[3].position = glm::vec3(0.0, 0, -1.5);
	(*scene->getMeshes())[4].position = glm::vec3(0.0, 1.5, 0.0);
	(*scene->getMeshes())[5].position = glm::vec3(0.0, -0.5, 0.0);
	(*scene->getMeshes())[6].position = glm::vec3( 1.5, 0.0, 0.0);
	(*scene->getMeshes())[7].position = glm::vec3(-1.5, 0.0, 0.0);
	(*scene->getMeshes())[8].position = glm::vec3(0.0, 1.49999, 0.0);

	//scale
	(*scene->getMeshes())[0].scale = glm::vec3(5.0);
	(*scene->getMeshes())[1].scale = glm::vec3(1.0);

	(*scene->getMeshes())[2].scale = glm::vec3(1.5);
	(*scene->getMeshes())[3].scale = glm::vec3(1.5);
	(*scene->getMeshes())[4].scale = glm::vec3(1.5);
	(*scene->getMeshes())[5].scale = glm::vec3(1.5);
	(*scene->getMeshes())[6].scale = glm::vec3(1.5);
	(*scene->getMeshes())[7].scale = glm::vec3(1.5);
	(*scene->getMeshes())[8].scale = glm::vec3(0.5);

	//rotation
	(*scene->getMeshes())[0].rotation = glm::vec3(0.0, 3.141592, 0.0);
	(*scene->getMeshes())[1].rotation = glm::vec3(0.0, 0.0, 0.0);

	(*scene->getMeshes())[2].rotation = glm::vec3(0.0, 0.0, 0.0);
	(*scene->getMeshes())[3].rotation = glm::vec3(0.0, 3.141592, 0.0);
	(*scene->getMeshes())[4].rotation = glm::vec3(3.141592 * 1.5, 0.0, 0.0);
	(*scene->getMeshes())[5].rotation = glm::vec3(3.141592 * 0.5, 0.0, 0.0);
	(*scene->getMeshes())[6].rotation = glm::vec3(0.0, 3.141592 * 0.5, 0.0);
	(*scene->getMeshes())[7].rotation = glm::vec3(0.0, 3.141592 * 1.5, 0.0);
	(*scene->getMeshes())[8].rotation = glm::vec3(3.141592 * 1.5, 0.0, 0.0);





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