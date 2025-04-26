#include "Scene/Scene.h"
#include "Core/Input.h"

#include <iostream>

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

void Scene::addMesh(ref<Mesh> mesh) {
	meshList.emplace_back(mesh);
}

void Scene::addModel(Model&& model) {
	for (const ref<Mesh>& mesh: model.getMeshes()) {
		meshList.emplace_back(mesh);
	}
}
