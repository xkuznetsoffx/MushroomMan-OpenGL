#include "Skybox.h"

Skybox::Skybox(const std::vector<std::string>& faces)
{
    float scale = 50.0f; // Скайбокс охватывает карту
    vertices = {
        -scale,  scale, -scale,
        -scale, -scale, -scale,
         scale, -scale, -scale,
         scale, -scale, -scale,
         scale,  scale, -scale,
        -scale,  scale, -scale,

        -scale, -scale,  scale,
        -scale, -scale, -scale,
        -scale,  scale, -scale,
        -scale,  scale, -scale,
        -scale,  scale,  scale,
        -scale, -scale,  scale,

         scale, -scale, -scale,
         scale, -scale,  scale,
         scale,  scale,  scale,
         scale,  scale,  scale,
         scale,  scale, -scale,
         scale, -scale, -scale,

        -scale, -scale,  scale,
        -scale,  scale,  scale,
         scale,  scale,  scale,
         scale,  scale,  scale,
         scale, -scale,  scale,
        -scale, -scale,  scale,

        -scale,  scale, -scale,
         scale,  scale, -scale,
         scale,  scale,  scale,
         scale,  scale,  scale,
        -scale,  scale,  scale,
        -scale,  scale, -scale,

        -scale, -scale, -scale,
        -scale, -scale,  scale,
         scale, -scale, -scale,
         scale, -scale, -scale,
        -scale, -scale,  scale,
         scale, -scale,  scale
    };
    initVAO();
    initCubemap(faces);
}

Skybox::~Skybox()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Skybox::render(Shader* shader)
{
    // draw skybox as last
    glDepthFunc(GL_LEQUAL); 
    shader->Use();

    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

void Skybox::initVAO()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void Skybox::initCubemap(const std::vector<std::string>& faces)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}
