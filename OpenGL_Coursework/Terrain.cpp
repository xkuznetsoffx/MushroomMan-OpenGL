#include "Terrain.h"

Terrain::Terrain(int width, int height, float scale, std::shared_ptr<Texture> texture)
	:
	width(width), height(height), scale(scale), texture(texture)
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

void Terrain::render(Shader* shader)
{
	shader->Use();

	shader->setMat4("model", glm::mat4(1.f));

	shader->setInt("material.diffuse", 0);
	shader->setInt("material.specular", 0);
	shader->setFloat("material.shininess", 16.f);

	texture->bindTexture(0);

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
	std::vector<GLfloat> normals;

	normals.resize(width * height * 3, 0.0f);

	for (int z = 0; z < height; ++z) {
		for (int x = 0; x < width; ++x) {
			//position
			vertices.push_back(static_cast<GLfloat>(x));
			vertices.push_back(heightMap[z * width + x]); 
			vertices.push_back(static_cast<GLfloat>(z)); 
			//texcoord
			vertices.push_back(static_cast<GLfloat>(x) / width);          
			vertices.push_back(static_cast<GLfloat>(z) / height);
			//normals
			vertices.push_back(static_cast<GLfloat>(0));
			vertices.push_back(static_cast<GLfloat>(1) );
			vertices.push_back(static_cast<GLfloat>(0) );
		}
	}

	for (int z = 0; z < height - 1; ++z) {
		for (int x = 0; x < width - 1; ++x) {
			int topLeft = z * width + x;
			int bottomLeft = (z + 1) * width + x;
			int topRight = z * width + (x + 1);
			int bottomRight = (z + 1) * width + (x + 1);

			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);

			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
			indices.push_back(topRight);

		}
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO  
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	// EBO  
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// Позиции  
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	// Текстурные координаты  
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Нормали  
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
