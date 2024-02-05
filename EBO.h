#pragma once

#include <glad/glad.h>
#include <vector>

struct EBO
{
	GLuint handle;

	EBO(const std::vector<GLuint>& vertices);

	~EBO();

	void bind();

	void unbind();
};
