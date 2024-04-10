#include <glad/glad.h>
#include "Mesh.h"

struct Person // TODO: Make this actually work
{
	GLdouble x;
	
	GLdouble y;

	GLdouble z;

	Mesh mesh;

	Person(GLdouble x, GLdouble y, GLdouble z);

	void draw(Shader& shader, Camera& camera);

	~Person();
};
