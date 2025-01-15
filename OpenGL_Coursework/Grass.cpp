#include "Grass.h"  
#include <random>  

Grass::Grass(Terrain* terrain, int grassCount, const std::string& path) : terrain(terrain) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> disX(0, terrain->getWidth()-2);
    std::uniform_real_distribution<> disZ(0, terrain->getHeight()-2);
    std::uniform_real_distribution<> disScale(0.4, 0.7);

    for (int i = 0; i < grassCount; ++i) {
        float x = disX(gen);
        float z = disZ(gen);
        float height = terrain->getCurrentHeightFromMap(x, z) - 0.05f;

        GrassInstance grass;
        grass.position = glm::vec3(x, height, z);
        grass.scale = disScale(gen);

        grassInstances.push_back(grass);
    }

    setupVAO();
    
    loadTexture();
    
}

void Grass::setupVAO() {
   
    std::vector<float> vertices = {
         
        -0.5f, 0.0f, 0.0f,  0.0f, 1.0f,
         0.5f, 0.0f, 0.0f,  1.0f, 1.0f,
        -0.5f, 1.0f, 0.0f,  0.0f, 0.0f,
         0.5f, 1.0f, 0.0f,  1.0f, 0.0f,

        
         0.0f, 0.0f, -0.5f,  0.0f, 1.0f,
         0.0f, 0.0f,  0.5f,  1.0f, 1.0f,
         0.0f, 1.0f, -0.5f,  0.0f, 0.0f,
         0.0f, 1.0f,  0.5f,  1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Позиционные атрибуты  
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Текстурные координаты  
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Grass::render(Shader* shader) {
    shader->Use();
    size_t i = 0;
    glDisable(GL_CULL_FACE);
    glBindVertexArray(VAO);

    int grassCnt = grassInstances.size() * 0.94f;
    int redFlowerCnt = grassInstances.size() * 0.04f;
    int yellowFlowerCnt = grassInstances.size() - grassCnt - redFlowerCnt;

    grassTexture->bindTexture(0);
    for (; i < grassCnt; ++i ) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, grassInstances[i].position);
        model = glm::scale(model, glm::vec3(grassInstances[i].scale));

        shader->setMat4("model", model);
 
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
    }

    redFlowerTexture->bindTexture(0);
    for (; i < grassCnt + redFlowerCnt; ++i) {
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model,
            glm::vec3(
                grassInstances[i].position.x,
                grassInstances[i].position.y + 0.05f,
                grassInstances[i].position.z
            )
            );
        model = glm::scale(model, glm::vec3(grassInstances[i].scale * 0.5f));

        shader->setMat4("model", model);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
    }

    yellowFlowerTexture->bindTexture(0);
    for (; i < grassCnt + redFlowerCnt + yellowFlowerCnt; ++i) {

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model,
            glm::vec3(
                grassInstances[i].position.x,
                grassInstances[i].position.y + 0.05f,
                grassInstances[i].position.z
            )
        );
        model = glm::scale(model, glm::vec3(grassInstances[i].scale * 0.5f));

        shader->setMat4("model", model);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
    }

    glBindVertexArray(0);
    glEnable(GL_CULL_FACE);
}

void Grass::loadTexture() {
    grassTexture = std::make_unique<Texture>("assets/textures/trava.png", GL_TEXTURE_2D, aiTextureType_DIFFUSE);
    redFlowerTexture = std::make_unique<Texture>("assets/textures/redFlower.png", GL_TEXTURE_2D, aiTextureType_DIFFUSE);
    yellowFlowerTexture = std::make_unique<Texture>("assets/textures/yellowFlower.png", GL_TEXTURE_2D, aiTextureType_DIFFUSE);
}

Grass::~Grass() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}