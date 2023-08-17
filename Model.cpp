#include "Model.h"

Model::Model(const char* path)
{
	LoadModel(path);
}

void Model::Draw(Shader& shader, Camera& camera)
{
	for (size_t i = 0; i < meshes.size(); i++)
	{
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, translation);
		trans = glm::rotate(trans, rotationRadians, rotationAxis);
		trans = glm::scale(trans, scale);

		glm::mat4 objectModelMatrix = matrices[i] * trans;
		// Set the normal matrix in the shader to calculate the proper normal direction
		shader.SetMat3("normalMatrix", glm::mat3(glm::transpose(glm::inverse(objectModelMatrix))));

		meshes[i].Draw(shader, camera, objectModelMatrix);
	}
}

void Model::LoadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
	}
	directory = path.substr(0, path.find_last_of('/'));

	if (scene)
		ProcessNode(scene->mRootNode, scene, glm::mat4(1.0f));
	
	std::cout << "Number of mesh: " << meshes.size() << std::endl;
	std::cout << "Number of loaded textures: " << texturesLoaded.size() << std::endl;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene, glm::mat4 matrix)
{
	// Store the transformation of the node
	aiMatrix4x4 mat = node->mTransformation;
	glm::mat4 matParent = glm::mat4(
		mat[0][0], mat[1][0], mat[2][0], mat[3][0],
		mat[0][1], mat[1][1], mat[2][1], mat[3][1],
		mat[0][2], mat[1][2], mat[2][2], mat[3][2],
		mat[0][3], mat[1][3], mat[2][3], mat[3][3]
	);
	glm::mat4 matNode = matrix * matParent;

	// Process all the node's meshes
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		// Store the mesh
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
		matrices.push_back(matNode);
	}
	// Then do the same for each of its childern
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, matNode);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	std::cout << mesh->mName.C_Str() << std::endl;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		// Process vertex position, normals and texture coordinates
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if (mesh->mTextureCoords[0])
			vertex.texureUV = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		else
			vertex.texureUV = glm::vec2(0.0f);

		vertices.push_back(vertex);
	}
	// Process indices
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	// Process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, textureType::DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, textureType::SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType aiTexType, textureType texType)
{
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < material->GetTextureCount(aiTexType); i++)
	{
		aiString str;
		material->GetTexture(aiTexType, i, &str);
		std::string filePath = str.C_Str();
		filePath = directory + '/' + filePath;

		bool skip = false;
		for (unsigned int j = 0; j < texturesLoaded.size(); j++)
		{
			if (texturesLoaded[j].path == filePath)
			{
				textures.push_back(texturesLoaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Texture texture(filePath.c_str(), texType, texturesLoaded.size());
			textures.push_back(texture);
			texturesLoaded.push_back(texture);
		}
	}
	return textures;
}