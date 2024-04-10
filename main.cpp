#if 1
#include "Model.h"

static constexpr GLint WINDOW_WIDTH = 800, WINDOW_HEIGHT = 800;

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Project1", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	Shader shader("default.vert", "default.frag");

	glm::vec4 lightColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec3 lightPos(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), lightPos);

	shader.activate();
	glUniform4f(glGetUniformLocation(shader.handle, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shader.handle, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glEnable(GL_DEPTH_TEST);

	Camera camera{ WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3{ 0.0f, 0.0f, 2.0f } };

	try {
		Model model("models/sword/scene.gltf");

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		bool escaped = false;

		GLenum error;
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // Navy blue
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (glfwGetKey(window, GLFW_KEY_ESCAPE))
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				escaped = true;
				camera.firstClick = true;
			}
			else if (escaped && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				escaped = false;
			}

			if (!escaped)
			{
				camera.handleMovementInputs(window);
			}
			camera.updateMatrix(90.0f, 0.1f, 100.0f);

			model.draw(shader, camera);

			glfwSwapBuffers(window);

			if ((error = glGetError()) != GL_NO_ERROR)
			{
				std::cout << "OpenGL Error: " << error << std::endl;
			}
		}

	}
	catch (std::exception e)
	{
		std::cerr << "Error: " << e.what();
	}

	// Cleanup
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
#else
#include "Mesh.h"

// Vertices coordinates
constexpr Vertex VERTICES[] =
{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
	Vertex{ glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
	Vertex{ glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
	Vertex{ glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
	Vertex{ glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f) }
};

// Indices for vertices order
constexpr GLuint INDICES[] =
{
	0, 1, 2,
	0, 2, 3
};

constexpr Vertex LIGHT_VERTICES[] =
{ //     COORDINATES     //
	Vertex{ glm::vec3(-0.1f, -0.1f,  0.1f) },
	Vertex{ glm::vec3(-0.1f, -0.1f, -0.1f) },
	Vertex{ glm::vec3(0.1f, -0.1f, -0.1f) },
	Vertex{ glm::vec3(0.1f, -0.1f,  0.1f) },
	Vertex{ glm::vec3(-0.1f,  0.1f,  0.1f) },
	Vertex{ glm::vec3(-0.1f,  0.1f, -0.1f) },
	Vertex{ glm::vec3(0.1f,  0.1f, -0.1f) },
	Vertex{ glm::vec3(0.1f,  0.1f,  0.1f) }
};

constexpr GLuint LIGHT_INDICES[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

constexpr GLint WINDOW_WIDTH = 800, WINDOW_HEIGHT = 800;

int main(int argc, const char *argv[])
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Project1", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	const Texture TEXTURES[] =
	{
		Texture{ "planks.png", "diffuse", 0 },
		Texture{ "planksSpec.png", "specular", 1 }
	};

	std::vector<Vertex> verts(VERTICES, VERTICES + sizeof(VERTICES) / sizeof(Vertex));
	std::vector<GLuint> ind(INDICES, INDICES + sizeof(INDICES) / sizeof(GLuint));
	std::vector<Texture> tex(TEXTURES, TEXTURES + sizeof(TEXTURES) / sizeof(Texture));

	Shader shader("default.vert", "default.frag");

	Mesh floor{ verts, ind, tex };

	Shader lightShader("light.vert", "light.frag");

	VAO lightVAO;

	std::vector<Vertex> lightVerts(LIGHT_VERTICES, LIGHT_VERTICES + sizeof(LIGHT_VERTICES) / sizeof(Vertex));
	std::vector<GLuint> lightInd(LIGHT_INDICES, LIGHT_INDICES + sizeof(LIGHT_INDICES) / sizeof(GLuint));

	Mesh light{ lightVerts, lightInd, tex };

	glm::vec4 lightColor{ 1.0f, 1.0f, 1.0f, 1.0f };

	glm::vec3 lightPos(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), lightPos);

	glm::vec3 pyramidPos(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::translate(glm::mat4(1.0f), pyramidPos);

	lightShader.activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.handle, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.handle, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	shader.activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.handle, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shader.handle, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shader.handle, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Camera camera{ WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3{ 0.0f, 0.0f, 2.0f } };

	bool escaped = false;

	GLenum error;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // Navy blue
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			escaped = true;
			camera.firstClick = true;
		}
		else if (escaped && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			escaped = false;
		}

		if (!escaped)
		{
			camera.handleMovementInputs(window);
		}
		camera.updateMatrix(90.0f, 0.1f, 100.0f);

		floor.draw(shader, camera);
		light.draw(lightShader, camera);

		glfwSwapBuffers(window);

		if ((error = glGetError()) != GL_NO_ERROR)
		{
			std::cout << "OpenGL Error: " << error << std::endl;
		}
	}

	// Cleanup
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
#endif
