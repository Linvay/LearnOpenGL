#include "Mesh.h"

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

	VAO.Bind();
	VBO VBO(vertices);
	EntityBufferObject EBO(indices);
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;

	VAO.Bind();
	VBO VBO(vertices);
	EntityBufferObject EBO(indices);
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Mesh::Draw(Shader& shader, Camera& camera)
{
	shader.Activate();
	VAO.Bind();

	size_t numDiffuse = 0;
	size_t numSpecular = 0;

	for (size_t i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string name;
		textureType type = textures[i].type;

		if (type == DIFFUSE)
		{
			num = std::to_string(numDiffuse++);
			name = "textureDiffuse" + num;
		}
		else if (type == SPECULAR)
		{
			num = std::to_string(numSpecular++);
			name = "textureSpecular" + num;
		}

		textures[i].SetTextureUnit(shader, name.c_str(), i);
		textures[i].Bind();
	}

	shader.SetVec3("cameraPosition", camera.Position);
	shader.SetVec3("spotLightPosition", camera.Position);
	shader.SetVec3("spotLightDirection", camera.Orientation);
	camera.SetShaderMatrix(shader, "camera");

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}