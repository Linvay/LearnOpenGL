#pragma once

#include <string>

#include "VertexArray.h"
#include "EntityBuffer.h"
#include "Camera.h"
#include "Texture.h"

class Mesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;

	VertexArray VAO;

	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices);
	void Draw(
		Shader& shader,
		Camera& camera,
		glm::mat4 matrix = glm::mat4(1.0f)
	);
};