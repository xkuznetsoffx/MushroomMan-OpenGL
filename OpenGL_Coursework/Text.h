#pragma once

#include <iostream>  
#include <string>  
#include <map>  

#include <glm/glm.hpp>  

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.h"

struct Character {
    GLuint TextureID;   // ID �������� �����  
    glm::ivec2 Size;    // ������ �����  
    glm::ivec2 Bearing; // �������� �� ������� �����  
    GLuint Advance;     // �������������� �������� ��� ���������� �����  
};

class Text
{
public:
	Text(const std::string& fontPath, int fontSize = 4);
    ~Text();

	void render(Shader* shader, const std::string& text,
        float x, float y, float scale,
        const glm::vec3& color = glm::vec3(1.0f));

private:
    FT_Library ft;
    FT_Face face;

    GLuint VAO, VBO;

    std::map<GLchar, Character> Characters;

    void initFreeType(const std::string& fontPath, int fontSize);
    void initVAO();
};

