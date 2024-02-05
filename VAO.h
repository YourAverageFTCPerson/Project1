#pragma once

#include <glad/glad.h>
#include "VBO.h"

// Stride is the distance between the end of one layout thing and the start of another of the same.
void linkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizei stride, void* offset);

struct VAO
{
	GLuint handle;

	VAO();

	~VAO();

	void bind();

	void unbind();
};
