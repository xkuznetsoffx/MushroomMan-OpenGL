#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <SOIL2/SOIL2.h>
#include <SOIL2/stb_image.h>
#include "assimp/types.h"
#include "assimp/material.h"

class Texture
{
public:
	Texture(const char* path, GLenum type, aiTextureType aiTexTypeName, bool isTransparent = false);
	~Texture();

	GLuint getID() const;
	aiTextureType getTypeName() const;
	std::string getPath() const;

	void bindTexture(const GLuint textureUnit);
	void unbindTexture();
	void loadFromFile(const char* path);

private:
	GLuint textureId;
	std::string path;
	GLenum textureType;
	aiTextureType aiTexTypeName;
	int width;
	int height;
};


