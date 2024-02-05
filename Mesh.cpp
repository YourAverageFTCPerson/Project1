#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures) : vertices{ vertices }, indices{ indices }, textures{ textures }
{
	vao.bind();

	VBO vbo(vertices);

	EBO ebo(indices);

	linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	linkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(GLfloat)));
	linkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(GLfloat)));
	linkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(GLfloat)));

	vao.unbind();

	ebo.unbind();
}

void Mesh::draw(Shader& shader, Camera& camera)
{
	GLenum error;

	shader.activate();
	vao.bind();

	size_t numDiffuse = 0;
	size_t numSpecular = 0;

	for (size_t i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		else
		{
			throw std::invalid_argument{ "Invalid texture type: " + type };
		}
		texUnit(shader, (type + num).c_str(), i);
		if ((error = glGetError()) != GL_NO_ERROR)
		{
			std::cerr << "texUnit OpenGL Error: " << error << std::endl;
		}
		textures[i].bind();
		if ((error = glGetError()) != GL_NO_ERROR)
		{
			std::cerr << "bind OpenGL Error: " << error << std::endl;
		}
	}
	glUniform3f(glGetUniformLocation(shader.handle, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.matrix(shader, "camMatrix");

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}
