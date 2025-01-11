#pragma once

#include <iostream>
#include <vector>

#include "Shader.h"
#include "Primitives.h"
#include "Material.h"

class Mesh
{
public:
	Mesh(
		Primitive&& primitive,
		glm::vec3 position = glm::vec3(0.f),
		glm::vec3 rotation = glm::vec3(0.f),
		glm::vec3 scale = glm::vec3(1.0f),
		glm::vec3 origin = glm::vec3(0.f)
	);

	Mesh(
		Vertex* vertexArray, const unsigned nrOfVertices,
		GLuint* indexArray, const unsigned nrOfIndices,
		std::vector<std::shared_ptr<Texture>> textures,
		glm::vec3 position = glm::vec3(0.f),
		glm::vec3 rotation = glm::vec3(0.f),
		glm::vec3 scale = glm::vec3(1.0f),
		glm::vec3 origin = glm::vec3(0.0f)
	);


	Mesh(const Mesh& obj);

	~Mesh();

	glm::vec3 getPosition();

	void setPosition(const glm::vec3& position);
	void setRotation(const glm::vec3& rotation);
	void setScale(const glm::vec3& scale);
	void setOrigin(const glm::vec3& origin);

	void move(const glm::vec3 position);
	void rotate(const glm::vec3 rotation);
	void scaleUp(const glm::vec3 scale);


	void update();
	void render(Shader* shader);
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;


	unsigned nrOfVertices;
	unsigned nrOfIndices;

	std::vector<std::shared_ptr<Texture>> textures;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 origin;

	glm::mat4 model;

	void initVAO();

	void updateUniforms(Shader* shader);
	void updateModelMatrix();


};

