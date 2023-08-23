#include "EntityBuffer.h"

EntityBuffer::EntityBuffer(std::vector<GLuint>& indices)
{
	ID = 0;
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void EntityBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EntityBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EntityBuffer::Delete()
{
	glDeleteBuffers(1, &ID);
}