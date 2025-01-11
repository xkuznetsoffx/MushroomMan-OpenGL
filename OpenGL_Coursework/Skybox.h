#pragma once

#include <SOIL2/SOIL2.h>
#include <SOIL2/stb_image.h>

#include "Shader.h"

class Skybox
{
public:
	Skybox(const std::vector<std::string>& faces);
	~Skybox();

	void render(Shader* shader);

private:
	std::vector<GLfloat> vertices;
	GLuint VAO, VBO;
	GLuint textureID;

	void initVAO();
	void initCubemap(const std::vector<std::string>& faces);
};

