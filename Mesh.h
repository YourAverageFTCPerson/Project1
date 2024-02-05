#pragma once

#include <string>
#include <vector>

#include "VAO.h"
#include "EBO.h"
#include "VBO.h"
#include "Camera.h"
#include "Texture.h"

struct Mesh
{
	std::vector<Vertex> vertices;

	std::vector<GLuint> indices;

	std::vector<Texture> textures;

	VAO vao;

	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures);

	void draw(Shader& shader, Camera& camera);
};
