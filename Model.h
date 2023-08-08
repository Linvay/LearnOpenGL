#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class Model
{
public:
	Model(const char* path);
	void Draw(Shader& shader, Camera& camera);
private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> texturesLoaded;

	void LoadModel(std::string path);
	void ProcessNode(aiNode *node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh *mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType aiTexType, textureType texType);
};