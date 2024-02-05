#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
};

struct VBO
{
	GLuint handle;

	VBO(const std::vector<Vertex>& vertices);

	~VBO();

	void bind();

	void unbind();
};
