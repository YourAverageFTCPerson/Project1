#pragma once

#include <stb/stb_image.h>
#include <glad/glad.h>

#include "Shader.h"

void texUnit(Shader& shader, const char* uniform, GLint unit);

struct Texture
{
	GLuint handle = 0;

	const char* type;

	GLenum slot;

	Texture(const char* filename, const char* type, GLuint slot, bool flipImage = true);

	~Texture();

	void bind();

	void unbind();
};
