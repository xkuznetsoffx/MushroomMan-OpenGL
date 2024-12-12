#include "Shader.h"

Shader::Shader(const int versionMajor, const int versionMinor,
	const char* vertexFile, const char* fragmentFile, const char* geometryFile) 
	: versionMajor(versionMajor), versionMinor(versionMinor)
{
	GLuint vertexShader = 0;
	GLuint fragmentShader = 0;
	GLuint geometryShader = 0;

	vertexShader = loadShader(GL_VERTEX_SHADER, vertexFile);
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentFile);
	if (geometryFile != "")
		geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryFile);

	linkProgram(vertexShader, fragmentShader, geometryShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(geometryShader);
}



Shader::~Shader() {
	glDeleteProgram(ID);
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec2(const std::string& name, float x, float y) const {
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1,glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const {
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}


void Shader::setMat4(const std::string& name, const glm::mat4& mat, GLboolean transpose) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, transpose, glm::value_ptr(mat));
}

std::string Shader::loadShaderCode(const char* path) {
	std::ifstream inFile(path);
	std::string shaderCode;

	if (!inFile.is_open()) {
		std::cerr <<"ERROR:Shader.CPP::COULD_NOT_OPEN_SHADERFILE" << path << '\n';
		return std::string();
	}

	std::string line;
	while (std::getline(inFile, line)) {
		shaderCode += line + '\n';
	}

	inFile.close();

	std::string versionNr =
		std::to_string(this->versionMajor) +
		std::to_string(this->versionMinor) +
		"0";

	shaderCode.replace(shaderCode.find("#version"), 12, ("#version " + versionNr));

	return shaderCode;
}

GLuint Shader::loadShader(GLenum shaderType, const char* path) {
	char infoLog[512];
	GLint success;

	GLuint shader;
	std::string shaderStr = loadShaderCode(path);
	const char* shaderSrc = shaderStr.c_str();
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSrc, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
		std::cerr << "ERROR:Shader.CPP::COULD_NOT_COMPILE_SHADER " << path << '\n';
		std::cerr << infoLog << '\n';
	}
	return shader;
}

void Shader::linkProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader) {
	char infoLog[512];
	GLint success;

	ID = glCreateProgram();

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	if (geometryShader)
		glAttachShader(ID, geometryShader);

	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, sizeof(infoLog), NULL, infoLog);
		std::cerr << "ERROR:Shader.CPP::COULD_NOT_LINK_PROGRAM" << '\n';
		std::cerr << infoLog << '\n';
	}

	glUseProgram(0);
}

void Shader::Use() {
	glUseProgram(ID);
}

void Shader::UnUse() {
	glUseProgram(0);
}
