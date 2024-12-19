#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"


class Material
{
public:
	Material(GLint diffuse, GLint specular, float shininess);
	~Material();

	void sendToShader(Shader* program);	

private:
	GLint diffuse;
	GLint specular;
	float shininess;
};

