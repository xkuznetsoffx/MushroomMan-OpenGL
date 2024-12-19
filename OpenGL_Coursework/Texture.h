#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <SOIL2/SOIL2.h>

class Texture
{
public:
	Texture(const char* path, GLenum type);
	~Texture();

	GLuint getID() const;

	void bindTexture(const GLuint textureUnit);
	void unbindTexture();
	void loadFromFile(const char* path);

private:
	GLuint textureId;
	GLenum textureType;
	int width;
	int height;
};


