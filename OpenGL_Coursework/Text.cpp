#include "Text.h"

Text::Text(const std::string& fontPath, int fontSize)
{
	initFreeType(fontPath, fontSize);
	initVAO();
}

Text::~Text()
{
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
 
    for (auto& character : Characters) {
        glDeleteTextures(1, &character.second.TextureID);
    }
}

void Text::render(Shader* shader, const std::string& text, float x, float y, float scale, const glm::vec3& color)
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    shader->Use();
    shader->setVec3("textColor", color);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Рендеринг каждого символа  
    for (auto c : text) {
        Character ch = Characters[c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        // Обновление VBO для каждого символа  
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        // Привязка текстуры символа  
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // Обновление содержимого VBO  
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Рендеринг символа  
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Смещение для следующего символа  
        x += (ch.Advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Text::initFreeType(const std::string& fontPath, int fontSize)
{
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        throw std::runtime_error("FreeType initialization failed");
    }

    // Загрузка шрифта  
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
        throw std::runtime_error("Font loading failed");
    }

    // Установка размера шрифта  
    FT_Set_Pixel_Sizes(face, 0, fontSize);

    // Отключение привязки байтов для текстуры   
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Загрузка глифов ASCII символов  
    for (GLubyte c = 0; c < 128; c++) {
        // Загрузка символа  
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
            continue;
        }

        // Создание текстуры для символа  
        GLuint texture;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Настройка параметров текстуры  
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        // Сохранение характеристик символа  
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
}

void Text::initVAO()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
