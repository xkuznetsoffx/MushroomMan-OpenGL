#pragma once

#include <vector>  
#include <iostream>  
#include <memory>
#include "../packages/FastNoiseLite.h"

#include "Material.h"
#include "Vertex.h"
#include "Shader.h"

class Terrain
{
public:
	Terrain(int width, int height, float scale, Material* material);
	~Terrain();
	void render(Shader* shader);
	float getHeight(float x, float z);
	void generateTerrain();

private:
	std::vector<float> heightMap;
	int width;
	int height;
	float scale;
	Material* material;
	std::shared_ptr<Texture> texture;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	unsigned int nrOfIndices;

	void initVAO();

	bool isCoordInMap(float x, float z);

};

