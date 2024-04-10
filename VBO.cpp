#include "VBO.h"

VBO::VBO(const std::vector<Vertex>& vertices)
{
	glGenBuffers(1, &handle);
	glBindBuffer(GL_ARRAY_BUFFER, handle);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

VBO::~VBO()
{
	glDeleteBuffers(1, &handle);
}

void VBO::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, handle);
}

void VBO::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
}
