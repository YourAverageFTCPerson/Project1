#pragma once

#include <json/json.hpp>
#include "Mesh.h"

using json = nlohmann::json;

template<typename VecType>
std::vector<VecType> groupFloats(std::vector<GLfloat> floats, size_t components);

#define groupFloatsVec2(floats) groupFloats<glm::vec2>(floats, 2)
#define groupFloatsVec3(floats) groupFloats<glm::vec3>(floats, 3)
#define groupFloatsVec4(floats) groupFloats<glm::vec4>(floats, 4)

std::vector<Vertex> assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUVs);

class Model
{
public:
	Model(const char* file);

	void draw(Shader& shader, Camera& camera);

private:
	const char* file;

	std::vector<stbi_uc> data;

	json jsonObject;
	
	std::vector<stbi_uc> getData();
	std::vector<GLfloat> getFloats(json accessor);
	std::vector<GLuint> getIndices(json accessor);
	std::vector<Texture> getTextures();
};
