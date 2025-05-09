#pragma once

#include "Core/Defines.h"
#include "Scene/Mesh.h"
#include "Scene/Model.h"

class Scene {
public:
	Scene();
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	Scene(Scene&&) = delete;
	Scene& operator=(Scene&&) = delete;

	void addMesh(Mesh&& mesh);
	void addModel(const Model& model, unsigned material, const glm::vec3& color, float value1, float value);

	void update(float dt);

	inline const Camera& getCamera() const { return camera; }
	inline const ref<list<Mesh>> getMeshes() const { return meshList; }

private:
	Camera camera;
	ref<list<Mesh>> meshList;
};