#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <iostream>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texureUV;
};

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec);

class VertexBuffer
{
public:
	GLuint ID;
	VertexBuffer(std::vector<Vertex>& vertices);

	void Bind();
	void Unbind();
	void Delete();
};