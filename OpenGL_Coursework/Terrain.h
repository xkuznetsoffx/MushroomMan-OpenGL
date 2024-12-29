#pragma once

#include <vector>  
#include <iostream>  
#include <memory>
#include "../packages/FastNoiseLite.h"

#include "Texture.h"
#include "Vertex.h"
#include "Shader.h"

class Terrain
{
public:
	Terrain(int width, int height, float scale, std::shared_ptr<Texture> texture);
	~Terrain();
	void render(Shader* shader);

	void generateTerrain();

private:
	std::vector<float> heightMap;
	int width;
	int height;
	float scale;

	std::shared_ptr<Texture> texture;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	void initVAO();

};

