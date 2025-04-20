#include "Scene.h"

Scene::Scene() {
}

void Scene::render() {

}

void Scene::renderRayTrace() {
}

void Scene::addMesh(Mesh&& mesh) {
	meshList.emplace_back(mesh);
}

void Scene::addModel(Model&& model) {
	for (ref<Mesh> mesh: model.getMeshes()) {
		meshList.emplace_back(mesh);
	}
}
