#include "Mesh.h"

Mesh::Mesh(
	Primitive&& primitive,
	glm::vec3 position,
	glm::vec3 rotation,
	glm::vec3 scale,
	glm::vec3 origin
) 
	:
	position(position), rotation(rotation), scale(scale), origin(origin)
{
	this->nrOfVertices = primitive.getNrOfVertices();
	this->nrOfIndices = primitive.getNrOfIndices();

	vertices.assign(primitive.getVertices(), primitive.getVertices() + nrOfVertices);
	indices.assign(primitive.getIndices(), primitive.getIndices() + nrOfIndices);

	initVAO();
	updateModelMatrix();
}


Mesh::Mesh(
	Vertex* vertexArray, const unsigned nrOfVertices,
	GLuint* indexArray, const unsigned nrOfIndices,
	glm::vec3 position,
	glm::vec3 rotation,
	glm::vec3 scale, 
	glm::vec3 origin
) 
:
	position(position) , rotation(rotation), scale(scale), origin(origin)
{
	this->nrOfVertices = nrOfVertices;
	this->nrOfIndices = nrOfIndices;
	
	vertices.assign(vertexArray, vertexArray + nrOfVertices);
	indices.assign(indexArray, indexArray + nrOfIndices);

	initVAO();
	updateModelMatrix();
}

Mesh::Mesh(const Mesh& obj)
	: position(obj.position), rotation(obj.rotation), scale(obj.scale), origin(obj.origin)
{
	this->nrOfVertices = obj.nrOfVertices;
	this->nrOfIndices = obj.nrOfIndices;

	vertices.assign(obj.vertices.begin(), obj.vertices.end());
	indices.assign(obj.indices.begin(), obj.indices.end());

	initVAO();
	updateModelMatrix();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	if (nrOfIndices > 0)
	{
		glDeleteBuffers(1, &EBO);
	}
}

glm::vec3 Mesh::getPosition()
{
	return this->position;
}

void Mesh::setPosition(const glm::vec3 &position)
{
	this->position = position;
}

void Mesh::setRotation(const glm::vec3& rotation)
{
	this->rotation = rotation;
}

void Mesh::setScale(const glm::vec3& scale)
{
	this->scale = scale;
}

void Mesh::move(const glm::vec3 position)
{
	this->position += position;
}

void Mesh::rotate(const glm::vec3 rotation)
{
	this->rotation += rotation;
}

void Mesh::scaleUp(const glm::vec3 scale)
{
	this->scale += scale;
}

void Mesh::render(Shader* shader)
{
	shader->Use();

	updateModelMatrix();
	updateUniforms(shader);

	glBindVertexArray(VAO);

	if (!nrOfIndices)
		glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
	else
		glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);

}

void Mesh::initVAO()
{
	//Create VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//Create VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, nrOfVertices * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	//Create EBO
	if (nrOfIndices > 0)
	{
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, nrOfIndices * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	}
	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	//Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(3);
	//Texcoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);
	//Color
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void Mesh::updateUniforms(Shader* shader)
{
	shader->setMat4("model", model);
}

void Mesh::updateModelMatrix()
{
	this->model = glm::mat4(1.f);

	this->model = glm::translate(this->model, this->origin);

	this->model = glm::rotate(this->model, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
	this->model = glm::rotate(this->model, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
	this->model = glm::rotate(this->model, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
	
	this->model = glm::translate(this->model, -this->origin);

	this->model = glm::translate(this->model, this->position);
	this->model = glm::scale(this->model, this->scale);
	
}
