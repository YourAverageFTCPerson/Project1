#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures) : vertices{ vertices }, indices{ indices }, textures{ textures }
{
	vao.bind();

	VBO vbo(vertices);

	EBO ebo(indices);

	linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), static_cast<void*>(0));
	linkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(GLfloat)));
	linkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(GLfloat)));
	linkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(GLfloat)));

	vao.unbind();

	ebo.unbind();
}

void Mesh::draw(Shader& shader, Camera& camera, glm::mat4 matrix, glm::vec3 translation, glm::quat rotation, glm::vec3 scale)
{
	GLenum error;
	shader.activate();
	GLint prog = 0;
	vao.bind();

	GLint numDiffuse = 0;
	GLint numSpecular = 0;

	for (GLint i = 0; i < static_cast<GLint>(textures.size()); i++)
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
		if ((error = glGetError()) != GL_NO_ERROR)
		{
			std::cerr << "|  before before before: " << error << std::endl;
		}
		texUnit(shader, (type + num).c_str(), i);
		if ((error = glGetError()) != GL_NO_ERROR)
		{
			std::cerr << "|  before before: " << error << std::endl;
			glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
		}
		textures[i].bind();
		if ((error = glGetError()) != GL_NO_ERROR)
		{
			std::cerr << "|  before: " << error << std::endl;
			glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
			std::cout << "current program: " << prog << std::endl;
		}
	}
	glUniform3f(glGetUniformLocation(shader.handle, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.matrix(shader, "camMatrix");

	glm::mat4 trans = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 rot = glm::mat4_cast(rotation);
	glm::mat4 sca = glm::scale(glm::mat4(1.0f), scale);

	glUniformMatrix4fv(glGetUniformLocation(shader.handle, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(glGetUniformLocation(shader.handle, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(glGetUniformLocation(shader.handle, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
	glUniformMatrix4fv(glGetUniformLocation(shader.handle, "model"), 1, GL_FALSE, glm::value_ptr(matrix)); // The light cube is in the middle because of this.

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, (void*)0); // FIXME: 1282
	if ((error = glGetError()) != GL_NO_ERROR)
	{
		std::cerr << "|  " << error << std::endl;
	}
}
