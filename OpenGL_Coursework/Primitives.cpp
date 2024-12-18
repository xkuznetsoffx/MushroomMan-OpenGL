#include "Primitives.h"

void Primitive::set(const Vertex* vertices, const unsigned nrOfVertices, const GLuint* indices, const unsigned nrOfIndices)
{
	this->vertices.assign(vertices, vertices + nrOfVertices);
	this->indices.assign(indices, indices + nrOfIndices);
}

Cube::Cube() : Primitive()
{
	Vertex vertices[] = {
		// Задняя грань
		{{-0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
		{{-0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
		{{-0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},

		// Передняя грань
		{{-0.5f, -0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f,  0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, -0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},

		// Левая грань 
		{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},

		// Правая грань 
		{{ 0.5f,  0.5f,  0.5f}, {1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}},

		// Нижняя грань 
		{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}, {1.0f, 0.5f, 0.0f}},
		{{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}, {1.0f, 0.5f, 0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}, {1.0f, 0.5f, 0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}, {1.0f, 0.5f, 0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}, {1.0f, 0.5f, 0.0f}},
		{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}, {1.0f, 0.5f, 0.0f}},

		// Верхняя грань
		{{-0.5f,  0.5f, -0.5f}, {0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}, {0.5f, 0.0f, 0.5f}},
		{{ 0.5f,  0.5f, -0.5f}, {0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}, {0.5f, 0.0f, 0.5f}},
		{{ 0.5f,  0.5f,  0.5f}, {0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}, {0.5f, 0.0f, 0.5f}},
		{{ 0.5f,  0.5f,  0.5f}, {0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}, {0.5f, 0.0f, 0.5f}},
		{{-0.5f,  0.5f,  0.5f}, {0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}, {0.5f, 0.0f, 0.5f}},
		{{-0.5f,  0.5f, -0.5f}, {0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}, {0.5f, 0.0f, 0.5f}},
	};

	unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

	set(vertices, nrOfVertices, nullptr, 0);
}

Quad::Quad() : Primitive()
{
	Vertex vertices[] =
	{
		// Position                                // Normals                      // Texcoords                 // Color  
		glm::vec3(-0.5f, 0.5f, 0.f),             glm::vec3(0.f, 0.f, 1.f),    glm::vec2(0.f, 5.f),       glm::vec3(1.f, 0.f, 0.f),
		glm::vec3(-0.5f, -0.5f, 0.f),            glm::vec3(0.f, 0.f, 1.f),    glm::vec2(0.f, 0.f),       glm::vec3(0.f, 1.f, 0.f),
		glm::vec3(0.5f, -0.5f, 0.f),             glm::vec3(0.f, 0.f, 1.f),    glm::vec2(5.f, 0.f),       glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(0.5f, 0.5f, 0.f),              glm::vec3(0.f, 0.f, 1.f),    glm::vec2(5.f, 5.f),       glm::vec3(1.f, 1.f, 0.f)
	};
	unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

	GLuint indices[] =
	{
		2, 1, 0,	//Triangle 1
		3, 2, 0		//Triangle 2
	};
	unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

	this->set(vertices, nrOfVertices, indices, nrOfIndices);
}
