#include "Camera.h"

Camera::Camera(GLint width, GLint height, glm::vec3 position) : width{ width }, height{ height }, position{ position }
{
}

void Camera::updateMatrix(float fov, float zNear, float zFar)
{
	// The model matrix is for turning local coordinates into world coordinates. (Not here)
	glm::mat4 view = glm::lookAt(position, position + orientation, up); // For turning world coordinates into view coordinates.
	glm::mat4 projection = glm::perspective(glm::radians(fov), (float)width / height, zNear, zFar); // For turning view coordinates into clip coordinates.

	cameraMatrix = projection * view;
}

void Camera::matrix(Shader& shader, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.handle, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::handleMovementInputs(GLFWwindow* window)
{
	if (firstClick)
	{
		glfwSetCursorPos(window, width * 0.5, height * 0.5);
		firstClick = false;
	}

	double mouseX;
	double mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	float rotX = sensitivity * (float)(mouseY - (height * 0.5)) / height;
	float rotY = sensitivity * (float)(mouseX - (width * 0.5)) / width;

	// Calculates upcoming vertical change in the Orientation
	glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));

	// Decides whether or not the next vertical Orientation is legal or not
	if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
	{
		orientation = newOrientation;
	}

	// Rotates the Orientation left and right
	orientation = glm::rotate(orientation, glm::radians(-rotY), up);

	// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
	glfwSetCursorPos(window, width * 0.5, height * 0.5);

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
	{
		speed = SPRINTING_SPEED;
	}
	else
	{
		speed = WALKING_SPEED;
	}

	if (glfwGetKey(window, GLFW_KEY_W)) // GLFW_PRESS is 1 so this works
	{
		position += speed * orientation;
	}

	if (glfwGetKey(window, GLFW_KEY_A))
	{
		position += speed * -glm::normalize(glm::cross(orientation, up));
	}

	if (glfwGetKey(window, GLFW_KEY_S))
	{
		position += speed * -orientation;
	}

	if (glfwGetKey(window, GLFW_KEY_D))
	{
		position += speed * glm::normalize(glm::cross(orientation, up));
	}
	
	if (glfwGetKey(window, GLFW_KEY_SPACE))
	{
		position += speed * up;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
	{
		position += speed * -up;
	}
}
