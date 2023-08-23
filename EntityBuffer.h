#pragma once

#include <glad/glad.h>

#include <vector>

class EntityBuffer
{
public:
	GLuint ID;
	EntityBuffer(std::vector<GLuint>& indices);

	void Bind();
	void Unbind();
	void Delete();
};