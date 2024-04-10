#include "Person.h"

static Mesh createPersonMesh(GLdouble x, GLdouble y, GLdouble z)
{
	auto vertices = std::vector<Vertex>();
	auto indices = std::vector<GLuint>();
	auto textures = std::vector<Texture>();
	return Mesh{ vertices, indices, textures };
}

Person::Person(GLdouble x, GLdouble y, GLdouble z) : x{ x }, y{ y }, z{ z }, mesh{ createPersonMesh(x, y, z) }
{
}

void Person::draw(Shader& shader, Camera& camera)
{
	mesh.draw(shader, camera);
}

Person::~Person()
{
	mesh.~Mesh();
}
