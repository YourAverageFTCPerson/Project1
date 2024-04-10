#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Shader.h"

constexpr GLfloat SPRINTING_SPEED = 0.1f;
constexpr GLfloat WALKING_SPEED = 0.025f;

struct Camera
{
	glm::vec3 position;
	glm::vec3 orientation{ 0.0f, 0.0f, -1.0f };
	glm::vec3 up{ 0.0f, 1.0f, 0.0f };
	glm::mat4 cameraMatrix{ 1.0f };

	bool firstClick = true;

	GLint width;
	GLint height;

	GLfloat speed = WALKING_SPEED;
	GLfloat sensitivity = 60.0f;

	Camera(GLint width, GLint height, glm::vec3 position);

	void updateMatrix(GLfloat fov, GLfloat zNear, GLfloat zFar);

	void matrix(Shader& shader, const GLchar* uniform);

	void handleMovementInputs(GLFWwindow* window);
};
