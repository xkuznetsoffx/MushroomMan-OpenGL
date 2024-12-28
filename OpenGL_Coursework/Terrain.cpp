#include "Terrain.h"

Terrain::Terrain(int width, int height, float scale)
	:
	width(width), height(height), scale(scale)
{
	heightMap.resize(width * height);
	generateTerrain();
	initVAO();
}

Terrain::~Terrain()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Terrain::render()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (width - 1) * (height - 1) * 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Terrain::generateTerrain()
{
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetFrequency(scale);

	float value;

	for(int z = 0; z < height; ++z) {
		for (int x = 0; x < width; ++x) {
			value = noise.GetNoise(static_cast<float>(x), static_cast<float>(z));
			heightMap[z * width + x] = value * 5.f;
		}
	}
}

void Terrain::initVAO()
{
	std::vector<GLuint> indices;
	std::vector<GLfloat> vertices;

	for (int z = 0; z < height; ++z) {
		for (int x = 0; x < width; ++x) {
			vertices.push_back(static_cast<GLfloat>(x));
			vertices.push_back(heightMap[z * width + x]); 
			vertices.push_back(static_cast<GLfloat>(z)); 
		}
	}

	for (int y = 0; y < height - 1; ++y) {
		for (int x = 0; x < width - 1; ++x) {
			int topLeft = y * width + x;
			int bottomLeft = (y + 1) * width + x;
			int topRight = y * width + (x + 1);
			int bottomRight = (y + 1) * width + (x + 1);

			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);

			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
			indices.push_back(topRight);
		}
	}

	// Создание VAO  
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Создание VBO  
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	// Создание EBO  
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// Настройка атрибута вершин  
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Снятие связывания  
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
