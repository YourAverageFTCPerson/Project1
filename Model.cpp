
#include "Model.h"

template<typename V>
std::vector<V> groupFloats(const std::vector<GLfloat>& floats, const size_t& components) {
    if (floats.size() % components != 0) {
        throw std::invalid_argument{ "Invalid input size" };
    }

    std::vector<V> result;
    result.reserve(floats.size() / components);

    for (size_t i = 0; i < floats.size(); i += components) {
        V vec;
        for (size_t j = 0; j < components; ++j) {
            vec[j] = floats[i + j];
        }
        result.push_back(vec);
    }

    return result;
}
#if 0
std::vector<Vertex> assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUVs)
{
    std::vector<Vertex> vertices;
    for (size_t i = 0; i < positions.size(); i++)
    {
        vertices.push_back(Vertex{ positions[i], normals[i], glm::vec3{ 1.0f, 1.0f, 1.0f }, texUVs[i] });
    }
    return vertices;
}

Model::Model(const char* file) : file{ file }, jsonObject(json::parse(read_file(file))), data{ getData() }
{
    traverseNode(0);
}

void Model::draw(Shader& shader, Camera& camera)
{
    for (size_t i = 0; i < meshes.size(); i++)
    {
        meshes[i].draw(shader, camera, meshMatrices[i]);
    }
}

void Model::loadMesh(size_t indMesh)
{
    size_t posAccInd = jsonObject["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
    size_t normalAccInd = jsonObject["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
    size_t texAccInd = jsonObject["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
    size_t indAccInd = jsonObject["meshes"][indMesh]["primitives"][0]["indices"];

    std::vector<GLfloat> posVec = getFloats(jsonObject["accessors"][posAccInd]);
    std::vector<glm::vec3> positions = groupFloatsVec3(posVec);
    std::vector<GLfloat> normalVec = getFloats(jsonObject["accessors"][normalAccInd]);
    std::vector<glm::vec3> normals = groupFloatsVec3(normalVec);
    std::vector<GLfloat> texVec = getFloats(jsonObject["accessors"][texAccInd]);
    std::vector<glm::vec2> texUVs = groupFloatsVec2(texVec);

    std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
    std::vector<GLuint> indices = getIndices(jsonObject["accessors"][indAccInd]);
    std::vector<Texture> textures = getTextures();

    meshes.push_back(Mesh{ vertices, indices, textures });
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

void Model::traverseNode(size_t nextNode, glm::mat4 matrix)
{
    json node = jsonObject["nodes"][nextNode];

    glm::vec3 translation(0.0f, 0.0f, 0.0f);
    if (node.find("translation") != node.end())
    {
        GLfloat transValues[3];
        for (size_t i = 0; i < node["translation"].size(); i++)
        {
            transValues[i] = node["translation"][i];
        }
        translation = glm::make_vec3(transValues);
    }
    glm::quat rotation(1.0f, 0.0f, 0.0f, 0.0f);
    if (node.find("rotation") != node.end())
    {
        GLfloat rotValues[] =
        {
            node["rotation"][3],
            node["rotation"][0],
            node["rotation"][1],
            node["rotation"][2]
        };
        rotation = glm::make_quat(rotValues);
    }
    glm::vec3 scale(1.0f, 1.0f, 1.0f);
    if (node.find("scale") != node.end())
    {
        GLfloat scaleValues[3];
        for (size_t i = 0; i < node["scale"].size(); i++)
        {
            scaleValues[i] = node["scale"][i];
        }
        scale = glm::make_vec3(scaleValues);
    }
    glm::mat4 matNode(1.0f);
    if (node.find("matrix") != node.end())
    {
        GLfloat matValues[16];
        for (size_t i = 0; i < node["matrix"].size(); i++)
        {
            matValues[i] = node["matrix"][i];
        }
        matNode = glm::make_mat4(matValues);
    }

    glm::mat4 trans = glm::translate(glm::mat4(1.0f), translation);
    glm::mat4 rot = glm::mat4_cast(rotation);
    glm::mat4 sca = glm::scale(glm::mat4(1.0f), scale);

    glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

    if (node.find("mesh") != node.end())
    {
        meshTranslations.push_back(translation);
        meshRotations.push_back(rotation);
        meshScales.push_back(scale);
        meshMatrices.push_back(matNextNode);

        loadMesh(node["mesh"]);
    }

    if (node.find("children") != node.end())
    {
        for (size_t i = 0; i < node["children"].size(); i++)
        {
            traverseNode(node["children"][i], matNextNode);
        }
    }
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

    size_t numPerVert = type == "SCALAR" ? 1 : type == "VEC2" ? 2 : type == "VEC3" ? 3 : type == "VEC4" ? 4 : throw std::invalid_argument{ "Invalid type. (Not SCALAR, VEC2, VEC3 or VEC4)" };

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

    size_t bufViewInd = accessor.value("bufferView", 0);
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
            indices.push_back(static_cast<GLuint>(value));
        }
        break;
    case 5122: // short
        for (size_t i = beginningOfData; i < byteOffset + accByteOff + count * 2;)
        {
            stbi_uc bytes[] = { data[i++], data[i++] };
            GLshort value;
            std::memcpy(&value, bytes, sizeof(GLshort));
            indices.push_back(static_cast<GLuint>(value));
        }
        break;
    default:
        throw std::runtime_error{ "Unrecognized componentType: " + componentType };
        // don't forget to put a break here in the future!
    }

    return indices;
}

std::vector<Texture> Model::getTextures()
{
    std::vector<Texture> textures;

    std::string fileStr{ file };
    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

    bool skip;
    for (size_t i = 0; i < jsonObject["images"].size(); i++)
    {
        std::string texPath = jsonObject["images"][i]["uri"];

        skip = false;
        for (size_t j = 0; j < loadedTexNames.size(); j++)
        {
            if (loadedTexNames[j] == texPath)
            {
                textures.push_back(loadedTextures[j]);
                skip = true;
                break;
            }
        }

        if (skip)
        {
            continue;
        }

        if (texPath.find("baseColor") != std::string::npos)
        {
            Texture diffuse{ (fileDirectory + texPath).c_str(), "diffuse", static_cast<GLuint>(loadedTextures.size()), false };
            textures.push_back(diffuse);
            loadedTextures.push_back(diffuse);
        }
        else if (texPath.find("metallicRoughness") != std::string::npos)
        {
            Texture specular{ (fileDirectory + texPath).c_str(), "specular", static_cast<GLuint>(loadedTextures.size()), false };
            textures.push_back(specular);
            loadedTextures.push_back(specular);
        }
        else
        {
            throw std::runtime_error{ "Unrecongnized texture path name" };
        }
        loadedTexNames.push_back(texPath);
    }

    return textures;
}
#else
#include "Model.h"

Model::Model(const char* file)
{
	// Make a jsonObject object
	std::string text = read_file(file);
	jsonObject = json::parse(text);

	// Get the binary data
	Model::file = file;
	data = getData();

	// Traverse all nodes
	traverseNode(0);
}

void Model::draw(Shader& shader, Camera& camera)
{
	// Go over all meshes and draw each one
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Mesh::draw(shader, camera, meshMatrices[i]);
	}
}

void Model::loadMesh(size_t indMesh)
{
	// Get all accessor indices
	unsigned int posAccInd = jsonObject["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
	unsigned int normalAccInd = jsonObject["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
	unsigned int texAccInd = jsonObject["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int indAccInd = jsonObject["meshes"][indMesh]["primitives"][0]["indices"];

	// Use accessor indices to get all vertices components
	std::vector<float> posVec = getFloats(jsonObject["accessors"][posAccInd]);
	std::vector<glm::vec3> positions = groupFloatsVec3(posVec);
	std::vector<float> normalVec = getFloats(jsonObject["accessors"][normalAccInd]);
	std::vector<glm::vec3> normals = groupFloatsVec3(normalVec);
	std::vector<float> texVec = getFloats(jsonObject["accessors"][texAccInd]);
	std::vector<glm::vec2> texUVs = groupFloatsVec2(texVec);

	// Combine all the vertex components and also get the indices and textures
	std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
	std::vector<GLuint> indices = getIndices(jsonObject["accessors"][indAccInd]);
	std::vector<Texture> textures = getTextures();

	// Combine the vertices, indices, and textures into a mesh
	meshes.push_back(Mesh(vertices, indices, textures));
}

void Model::traverseNode(size_t nextNode, glm::mat4 matrix)
{
	// Current node
	json node = jsonObject["nodes"][nextNode];

	// Get translation if it exists
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	if (node.find("translation") != node.end())
	{
		float transValues[3];
		for (unsigned int i = 0; i < node["translation"].size(); i++)
			transValues[i] = (node["translation"][i]);
		translation = glm::make_vec3(transValues);
	}
	// Get quaternion if it exists
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (node.find("rotation") != node.end())
	{
		float rotValues[4] =
		{
			node["rotation"][3],
			node["rotation"][0],
			node["rotation"][1],
			node["rotation"][2]
		};
		rotation = glm::make_quat(rotValues);
	}
	// Get scale if it exists
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	if (node.find("scale") != node.end())
	{
		float scaleValues[3];
		for (unsigned int i = 0; i < node["scale"].size(); i++)
			scaleValues[i] = (node["scale"][i]);
		scale = glm::make_vec3(scaleValues);
	}
	// Get matrix if it exists
	glm::mat4 matNode = glm::mat4(1.0f);
	if (node.find("matrix") != node.end())
	{
		float matValues[16];
		for (unsigned int i = 0; i < node["matrix"].size(); i++)
			matValues[i] = (node["matrix"][i]);
		matNode = glm::make_mat4(matValues);
	}

	// Initialize matrices
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	// Use translation, rotation, and scale to change the initialized matrices
	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	// Multiply all matrices together
	glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

	// Check if the node contains a mesh and if it does load it
	if (node.find("mesh") != node.end())
	{
		meshTranslations.push_back(translation);
		meshRotations.push_back(rotation);
		meshScales.push_back(scale);
		meshMatrices.push_back(matNextNode);

		loadMesh(node["mesh"]);
	}

	// Check if the node has children, and if it does, apply this function to them with the matNextNode
	if (node.find("children") != node.end())
	{
		for (unsigned int i = 0; i < node["children"].size(); i++)
			traverseNode(node["children"][i], matNextNode);
	}
}

std::vector<unsigned char> Model::getData()
{
	// Create a place to store the raw text, and get the uri of the .bin file
	std::string bytesText;
	std::string uri = jsonObject["buffers"][0]["uri"];

	// Store raw text data into bytesText
	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
	bytesText = read_file((fileDirectory + uri).c_str());

	// Transform the raw text data into bytes and put them in a vector
	std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
	return data;
}

std::vector<float> Model::getFloats(json accessor)
{
	std::vector<float> floatVec;

	// Get properties from the accessor
	unsigned int buffViewInd = accessor.value("bufferView", 1);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	std::string type = accessor["type"];

	// Get properties from the bufferView
	json bufferView = jsonObject["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	// Interpret the type and store it into numPerVert
	unsigned int numPerVert;
	if (type == "SCALAR") numPerVert = 1;
	else if (type == "VEC2") numPerVert = 2;
	else if (type == "VEC3") numPerVert = 3;
	else if (type == "VEC4") numPerVert = 4;
	else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");

	// Go over all the bytes in the data at the correct place using the properties from above
	unsigned int beginningOfData = byteOffset + accByteOffset;
	unsigned int lengthOfData = count * 4 * numPerVert;
	for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i)
	{
		unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
		float value;
		std::memcpy(&value, bytes, sizeof(float));
		floatVec.push_back(value);
	}

	return floatVec;
}

std::vector<GLuint> Model::getIndices(json accessor)
{
	std::vector<GLuint> indices;

	// Get properties from the accessor
	unsigned int buffViewInd = accessor.value("bufferView", 0);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	unsigned int componentType = accessor["componentType"];

	// Get properties from the bufferView
	json bufferView = jsonObject["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	// Get indices with regards to their type: unsigned int, unsigned short, or short
	unsigned int beginningOfData = byteOffset + accByteOffset;
	if (componentType == 5125)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i)
		{
			unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
			unsigned int value;
			std::memcpy(&value, bytes, sizeof(unsigned int));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5123)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bytes[] = { data[i++], data[i++] };
			unsigned short value;
			std::memcpy(&value, bytes, sizeof(unsigned short));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5122)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bytes[] = { data[i++], data[i++] };
			short value;
			std::memcpy(&value, bytes, sizeof(short));
			indices.push_back((GLuint)value);
		}
	}

	return indices;
}

std::vector<Texture> Model::getTextures()
{
	std::vector<Texture> textures;

	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	// Go over all images
	for (unsigned int i = 0; i < jsonObject["images"].size(); i++)
	{
		// uri of current texture
		std::string texPath = jsonObject["images"][i]["uri"];

		// Check if the texture has already been loaded
		bool skip = false;
		for (unsigned int j = 0; j < loadedTexNames.size(); j++)
		{
			if (loadedTexNames[j] == texPath)
			{
				textures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}
		}

		// If the texture has been loaded, skip this
		if (!skip)
		{
			// Load diffuse texture
			if (texPath.find("baseColor") != std::string::npos)
			{
				Texture diffuse = Texture((fileDirectory + texPath).c_str(), "diffuse", loadedTextures.size());
				textures.push_back(diffuse);
				loadedTextures.push_back(diffuse);
				loadedTexNames.push_back(texPath);
			}
			// Load specular texture
			else if (texPath.find("metallicRoughness") != std::string::npos)
			{
				Texture specular = Texture((fileDirectory + texPath).c_str(), "specular", loadedTextures.size());
				textures.push_back(specular);
				loadedTextures.push_back(specular);
				loadedTexNames.push_back(texPath);
			}
		}
	}

	return textures;
}

std::vector<Vertex> assembleVertices
(
	std::vector<glm::vec3> positions,
	std::vector<glm::vec3> normals,
	std::vector<glm::vec2> texUVs
)
{
	std::vector<Vertex> vertices;
	for (int i = 0; i < positions.size(); i++)
	{
		vertices.push_back
		(
			Vertex
			{
				positions[i],
				normals[i],
				glm::vec3(1.0f, 1.0f, 1.0f),
				texUVs[i]
			}
		);
	}
	return vertices;
}
#endif
