#include "Model.h"

template<typename VecType>
std::vector<VecType> groupFloats(std::vector<GLfloat> floats, size_t components)
{
    if (floats.size() % components != 0) {
        // Handle invalid input size
        std::cerr << "Invalid input size" << std::endl;
        return {};
    }

    std::vector<VecType> result;
    result.reserve(floats.size() / components);

    for (size_t i = 0; i < floats.size(); i += components) {
        VecType vec;
        for (size_t j = 0; j < components; ++j) {
            vec[j] = floats[i + j];
        }
        result.push_back(vec);
    }

    return result;
}

std::vector<Vertex> assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUVs)
{
    std::vector<Vertex> vertices;
    for (size_t i = 0; i < positions.size(); i++)
    {
        vertices.push_back(Vertex{ positions[i], normals[i], glm::vec3{ 1.0f, 1.0f, 1.0f }, texUVs[i] });
    }
    return vertices;
}

Model::Model(const char* file) : file{ file }, jsonObject(read_file(file)), data{ getData() }
{
}

std::vector<stbi_uc> Model::getData()
{
	std::string uri = jsonObject["buffers"][0]["uri"];

	std::string fileStr(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
	std::string bytesText = read_file(fileDirectory + uri);

	std::vector<stbi_uc> data(bytesText.begin(), bytesText.end());
	return data;
}

std::vector<GLfloat> Model::getFloats(json accessor)
{
    std::vector<GLfloat> floatVec;

    size_t bufViewInd = accessor.value("bufferView", 1);
    size_t count = accessor["count"];
    size_t accByteOff = accessor.value("byteOffset", 0);
    std::string type = accessor["type"];

    json bufferView = jsonObject["bufferViews"][bufViewInd];
    size_t byteOffset = bufferView["byteOffset"];

    size_t numPerVert = type == "SCALAR" ? 1 : type == "VEC2" ? 2 : type == "VEC3" ? 3 : type == "VEC4" ? 4 : throw std::invalid_argument{ "Invalid type. (Not SCALAR, VEC2, VEC3 or VEC4" };

    size_t beginningOfData = byteOffset + accByteOff;
    size_t lengthOfData = count * 4 * numPerVert;
    for (size_t i = beginningOfData; i < beginningOfData + lengthOfData;)
    {
        stbi_uc bytes[] = { data[i++], data[i++], data[i++], data[i++] };
        float value;
        std::memcpy(&value, bytes, sizeof(GLfloat));
        floatVec.push_back(value);
    }

    return floatVec;
}

std::vector<GLuint> Model::getIndices(json accessor)
{
    std::vector<GLuint> indices;

    size_t bufViewInd = accessor.value("bufferView", 1);
    size_t count = accessor["count"];
    size_t accByteOff = accessor.value("byteOffset", 0);
    GLuint componentType = accessor["componentType"];

    json bufferView = jsonObject["bufferViews"][bufViewInd];
    size_t byteOffset = bufferView["byteOffset"];

    size_t beginningOfData = byteOffset + accByteOff;
    switch (componentType)
    {
    case 5125: // unsigned int
        for (size_t i = beginningOfData; i < byteOffset + accByteOff + count * 4;)
        {
            stbi_uc bytes[] = { data[i++], data[i++], data[i++], data[i++] };
            GLuint value;
            std::memcpy(&value, bytes, sizeof(GLuint));
            indices.push_back(value);
        }
        break;
    case 5123: // unsigned short
        for (size_t i = beginningOfData; i < byteOffset + accByteOff + count * 2;)
        {
            stbi_uc bytes[] = { data[i++], data[i++] };
            GLushort value;
            std::memcpy(&value, bytes, sizeof(GLushort));
            indices.push_back((GLuint)value);
        }
        break;
    case 5122: // short
        for (size_t i = beginningOfData; i < byteOffset + accByteOff + count * 2;)
        {
            stbi_uc bytes[] = { data[i++], data[i++] };
            GLshort value;
            std::memcpy(&value, bytes, sizeof(GLshort));
            indices.push_back((GLuint)value);
        }
        break;
    }
    
    return indices;
}

std::vector<Texture> Model::getTextures()
{
    return {};
}
