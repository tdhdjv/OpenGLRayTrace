#pragma once

#include <ASSIMP/Importer.hpp>
#include <ASSIMP/scene.h>
#include <ASSIMP/postprocess.h>

#include "Core/Defines.h"
#include "Scene/Mesh.h"

class Model {
public:
	Model(const std::string& path);

	inline const list<MeshData>& getMeshData() const { return meshes; }
private:
	list<MeshData> meshes;
	std::string directory;

	void loadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	MeshData processMesh(aiMesh* mesh, const aiScene* scene);
};