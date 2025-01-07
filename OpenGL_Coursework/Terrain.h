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
	void generateTerrain();

	int getHeight();
	int getWidth();
	
	float getCurrentHeightFromMap(float x, float z);

	bool isCoordInMap(float x, float z);

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

	

};

