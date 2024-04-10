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

void Mesh::draw(Shader& shader, Camera& camera, glm::mat4 matrix, glm::vec3 translation, glm::quat rotation, glm::vec3 scale)
{
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
		textures[i].bind();
	}
	glUniform3f(glGetUniformLocation(shader.handle, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.matrix(shader, "camMatrix");

	glm::mat4 trans = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 rot = glm::mat4_cast(rotation);
	glm::mat4 sca = glm::scale(glm::mat4(1.0f), scale);
	
	glUniformMatrix4fv(glGetUniformLocation(shader.handle, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(glGetUniformLocation(shader.handle, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(glGetUniformLocation(shader.handle, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
	glUniformMatrix4fv(glGetUniformLocation(shader.handle, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}
