#pragma once

#include <iostream>
#include <vector>

#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Vertex.h"

class Mesh
{
public:
	Mesh(Vertex* vertexArray, const unsigned nrOfVertices,
		GLuint* indexArray, const unsigned nrOfIndices,
		glm::vec3 position = glm::vec3(0.f),
		glm::vec3 rotation = glm::vec3(0.f),
		glm::vec3 scale = glm::vec3(1.0f));

	~Mesh();

	void setPosition(const glm::vec3& position);
	void setRotation(const glm::vec3& rotation);
	void setScale(const glm::vec3& scale);

	void move(const glm::vec3 position);
	void rotate(const glm::vec3 rotation);
	void doScale(const glm::vec3 scale);


	void update();
	void render(Shader* shader);
private:
	unsigned nrOfVertices;
	unsigned nrOfIndices;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 model;

	void initVAO (Vertex* vertexArray, GLuint* indexArray);

	void updateUniforms(Shader* shader);
	void updateModelMatrix();


};

