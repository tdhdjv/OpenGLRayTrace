#include "Scene/Scene.h"
#include "Core/Input.h"

#include <iostream>

Scene::Scene(): meshList(makeRef<list<Mesh>>()){}

void Scene::update(float dt) {
	float speed = 1.0;
	if (Input::isPressed(GLFW_KEY_W)) {
		camera.setPosition(camera.getPosition() + camera.getFront() * speed * dt);
	}

	if (Input::isPressed(GLFW_KEY_A)) {
		camera.setPosition(camera.getPosition() - camera.getRight() * speed * dt);
	}

	if (Input::isPressed(GLFW_KEY_S)) {
		camera.setPosition(camera.getPosition() - camera.getFront() * speed * dt);
	}

	if (Input::isPressed(GLFW_KEY_D)) {
		camera.setPosition(camera.getPosition() + camera.getRight() * speed * dt);
	}

	if (Input::isPressed(GLFW_KEY_SPACE)) {
		camera.setPosition(camera.getPosition() + glm::vec3(0.0, 1.0, 0.0) * speed * dt);
	}

	if (Input::isPressed(GLFW_KEY_LEFT_SHIFT)) {
		camera.setPosition(camera.getPosition() - glm::vec3(0.0, 1.0, 0.0) * speed * dt);
	}

	if (Input::isPressed(GLFW_KEY_RIGHT)) {
		camera.setYaw(camera.getYaw() - dt);
	}

	if (Input::isPressed(GLFW_KEY_LEFT)) {
		camera.setYaw(camera.getYaw() + dt);
	}


	if (Input::isPressed(GLFW_KEY_UP)) {
		camera.setPitch(camera.getPitch() + dt);
	}

	if (Input::isPressed(GLFW_KEY_DOWN)) {
		camera.setPitch(camera.getPitch() - dt);
	}
}

void Scene::addMesh(Mesh&& mesh) {
	meshList->emplace_back(std::move(mesh));
}


void Scene::addModel(const Model& model, unsigned material, const glm::vec3& color, float value1, float value2) {
	for (const MeshData& meshData: model.getMeshData()) {
		meshList->emplace_back(meshData, material, color, value1, value2);
	}
}
