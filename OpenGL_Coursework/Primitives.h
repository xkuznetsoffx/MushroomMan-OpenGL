#pragma once
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Vertex.h"


class Primitive
{
public:
	Primitive() {};
	virtual ~Primitive() {};

	void set(const Vertex* vertices, const unsigned nrOfVertices,
		const GLuint* indices, const unsigned nrOfIndices);

	Vertex* getVertices()	{ return vertices.data(); }
	GLuint* getIndices()	{ return indices.data(); }

	const size_t getNrOfVertices(){ return vertices.size(); }
	const size_t getNrOfIndices() { return indices.size(); }


private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

};

class Quad : public Primitive
{
public:
	Quad();
};

class Cube : public Primitive
{
public:
	Cube();
};

