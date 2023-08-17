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
	void Translate(const glm::vec3& trans)
	{
		translation = trans;
	}
	void Rotate(float angle, const glm::vec3& axis)
	{
		rotationRadians = glm::radians(angle);
		rotationAxis = axis;
	}
	void Scale(const glm::vec3& scale)
	{
		Model::scale = scale;
	}
private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> texturesLoaded;

	glm::vec3 translation = glm::vec3(0.0f);
	float rotationRadians = glm::radians(0.0f);
	glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
	std::vector<glm::mat4> matrices;

	void LoadModel(std::string path);
	void ProcessNode(aiNode *node, const aiScene* scene, glm::mat4 matrix);
	Mesh ProcessMesh(aiMesh *mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType aiTexType, textureType texType);
};