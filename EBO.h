#pragma once

#include <glad/glad.h>

#include <vector>

class EntityBufferObject
{
public:
	GLuint ID;
	EntityBufferObject(std::vector<GLuint>& indices);

	void Bind();
	void Unbind();
	void Delete();
};