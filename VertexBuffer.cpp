#include "VertexBuffer.h"

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
{
	os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return os;
}

VertexBuffer::VertexBuffer(std::vector<Vertex>& vertices)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Delete()
{
	glDeleteBuffers(1, &ID);
}