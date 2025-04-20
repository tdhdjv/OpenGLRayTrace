#pragma once

#include "Core/Defines.h"
#include "Scene/Mesh.h"
#include "Scene/Model.h"

class Scene {
public:
	Scene();

	void addMesh(Mesh&& mesh);
	void addModel(Model&& model);

	inline const list<ref<Mesh>> getMeshes() const { return meshList; }
private:
	list<ref<Mesh>> meshList;
};