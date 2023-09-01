#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class Model
{
public:
	Model(const char* path, bool flipTexture = true);
	void Draw(Shader& shader, Camera& camera, float scale = 1.0f);
	void Translate(const glm::vec3& trans)
	{
		transformation = glm::translate(transformation, trans);
	}
	void Rotate(float angle, const glm::vec3& axis)
	{
		transformation = glm::rotate(transformation, glm::radians(angle), axis);
	}
	void Scale(const glm::vec3& scale)
	{
		transformation = glm::scale(transformation, scale);
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
	glm::mat4 transformation = glm::mat4(1.0f);

	glm::vec3 aabbMin = glm::vec3(0.0f);
	glm::vec3 aabbMax = glm::vec3(0.0f);

	void LoadModel(std::string path, bool flipTexture);
	void ProcessNode(aiNode *node, const aiScene* scene, glm::mat4 matrix);
	Mesh ProcessMesh(aiMesh *mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType aiTexType, textureType texType, const aiScene* scene);
};

glm::vec3 getGlmVec3FromAiVec3(aiVector3D& vec);