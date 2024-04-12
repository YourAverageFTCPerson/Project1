#pragma once

#include <json/json.hpp>
#include "Mesh.h"

using json = nlohmann::json;

template<typename V>
static std::vector<V> groupFloats(const std::vector<GLfloat>& floats, const size_t& components);

#define groupFloatsVec2(floats) groupFloats<glm::vec2>(floats, 2)
#define groupFloatsVec3(floats) groupFloats<glm::vec3>(floats, 3)
#define groupFloatsVec4(floats) groupFloats<glm::vec4>(floats, 4)

std::vector<Vertex> assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUVs);

struct Model
{
	Model(const char* file);

	void draw(Shader& shader, Camera& camera);

private:
	const char* file;
	json jsonObject;
	std::vector<stbi_uc> data;

	std::vector<Mesh> meshes;
	std::vector<glm::vec3> meshTranslations;
	std::vector<glm::quat> meshRotations;
	std::vector<glm::vec3> meshScales;
	std::vector<glm::mat4> meshMatrices;

	std::vector<std::string> loadedTexNames;
	std::vector<Texture> loadedTextures;

	void loadMesh(size_t indMesh);
	
	void traverseNode(size_t nextNode, glm::mat4 matrix = glm::mat4{ 1.0f });

	std::vector<stbi_uc> getData();
	std::vector<GLfloat> getFloats(json accessor);
	std::vector<GLuint> getIndices(json accessor);
	std::vector<Texture> getTextures();
};
