#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	Shader(const int versionMajor, const int versionMinor, 
		const char* vertexFile, const char* fragmentFile, const char* geometryFile = "");

	~Shader();

	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	void setVec2(const std::string& name, float x, float y) const;
	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;

	void setMat4(const std::string& name, const glm::mat4& mat, GLboolean transpose = GL_FALSE) const;
	
	void Use();
	void UnUse();


private:
	GLuint ID;
	const int versionMajor;
	const int versionMinor;

	std::string loadShaderCode(const char* path);
	GLuint loadShader(GLenum shaderType, const char* path);
	void linkProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader);
};