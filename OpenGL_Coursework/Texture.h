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
	Texture(const char* path, GLenum type, GLint texture_unit);
	~Texture();

	GLuint getID() const;
	GLuint getUnit() const;

	void bindTexture();
	void unbindTexture();
	void loadFromFile(const char* path);

private:
	GLuint textureId;
	GLenum textureType;
	GLint textureUnit;
	int width;
	int height;
};

