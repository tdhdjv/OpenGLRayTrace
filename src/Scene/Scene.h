#pragma once

#include "Core/Defines.h"
#include "Scene/Mesh.h"
#include "Scene/Model.h"

class Scene {
public:
	Scene() = default;

	void addMesh(ref<Mesh> mesh);
	void addModel(Model&& model);

	void update(float dt);

	inline const Camera& getCamera() const { return camera; }
	inline const list<ref<Mesh>>& getMeshes() const { return meshList; }

private:
	Camera camera;
	list<ref<Mesh>> meshList;
};