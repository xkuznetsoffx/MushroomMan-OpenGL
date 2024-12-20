#include "Texture.h"

Texture::Texture(const char* path, GLenum type) : 
	textureType(type)
{
	unsigned char* image = SOIL_load_image(path, &width, &height, NULL, SOIL_LOAD_RGBA);

	glGenTextures(1, &textureId);
	glBindTexture(type, textureId);

	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image) {
		glTexImage2D(type, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(type);
	}
	else {
		std::cout << "ERROR::TEXTURE::CONSTRUCTOR()::TEXTURE_LOADING_FAILED: " << path << "\n";

	}

	glActiveTexture(0);
	glBindTexture(type, 0);
	SOIL_free_image_data(image);
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureId);
}

GLuint Texture::getID() const {
	return textureId;
}


void Texture::bindTexture(const GLuint textureUnit) {
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(textureType, textureId);
}

void Texture::unbindTexture() {
	glActiveTexture(0);
	glBindTexture(textureType, 0);
}

void Texture::loadFromFile(const char* path) {
	if (textureId) {
		glDeleteTextures(1, &textureId);
	}
	unsigned char* image = SOIL_load_image(path, &width, &height, NULL, SOIL_LOAD_RGBA);

	glGenTextures(1, &textureId);
	glBindTexture(textureType, textureId);

	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image) {
		glTexImage2D(textureType, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(textureType);
	}
	else {
		std::cout << "ERROR::TEXTURE::LOAD_FROM_FILE()::TEXTURE_LOADING_FAILED: " << path << "\n";

	}

	glActiveTexture(0);
	glBindTexture(textureType, 0);
	SOIL_free_image_data(image);
}
