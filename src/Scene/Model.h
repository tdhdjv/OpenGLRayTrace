#pragma once

#include <ASSIMP/Importer.hpp>
#include <ASSIMP/scene.h>
#include <ASSIMP/postprocess.h>

#include "Renderer/Shader.h"
#include "Core/Defines.h"
#include "Scene/Mesh.h"

class Model {
public:
	Model(const std::string& path);

	const list<ref<Mesh>>& getMeshes() { return meshes; }
private:
	list<ref<Mesh>> meshes;
	std::string directory;


	void loadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	ref<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
};