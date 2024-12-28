#pragma once

#include <vector>  
#include <iostream>  
#include "../packages/FastNoiseLite.h"

#include "Vertex.h"
#include "Shader.h"

class Terrain
{
public:
	Terrain(int width, int height, float scale);
	~Terrain();
	void render();

	void generateTerrain();

private:
	std::vector<float> heightMap;
	int width;
	int height;
	float scale;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	void initVAO();

};

