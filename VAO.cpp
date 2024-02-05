#include "VAO.h"

void linkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizei stride, void* offset)
{
	vbo.bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	vbo.unbind();
}

VAO::VAO()
{
	glGenVertexArrays(1, &handle);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &handle);
}

void VAO::bind()
{
	glBindVertexArray(handle);
}

void VAO::unbind()
{
	glBindVertexArray(0);
}
