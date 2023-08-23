#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &ID);
}

void VertexArray::LinkAttrib(VertexBuffer& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VertexArray::Bind()
{
	glBindVertexArray(ID);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}

void VertexArray::Delete()
{
	glDeleteVertexArrays(1, &ID);
}