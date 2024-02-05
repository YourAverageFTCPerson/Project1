#include "Texture.h"

void texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	shader.activate();
	glUniform1i(glGetUniformLocation(shader.handle, uniform), unit);
}

Texture::Texture(const char* filename, const char* type, GLuint slot, bool flipImage) : type{ type }, slot{ GL_TEXTURE0 + slot }
{
	int imgWidth, imgHeight, numColCh;
	if (flipImage)
		stbi_set_flip_vertically_on_load(true); // remember in .gltfs to not do this
	stbi_uc* data = stbi_load(filename, &imgWidth, &imgHeight, &numColCh, 0);
	if (!data)
		throw std::invalid_argument{ "Failed to load texture" };

	glGenTextures(1, &handle);
	glActiveTexture(this->slot);
	glBindTexture(GL_TEXTURE_2D, handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// float flatColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	// glTexParameterfv(texType, GL_TEXTURE_BORDER_COLOR, flatColor);

	switch (numColCh)
	{
	case 4:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		break;
	case 3:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		break;
	case 1:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		break;
	default:
		throw std::invalid_argument{ "Invalid numColCh" };
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &handle);
}

void Texture::bind()
{
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, handle);
}

void Texture::unbind()
{
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}
