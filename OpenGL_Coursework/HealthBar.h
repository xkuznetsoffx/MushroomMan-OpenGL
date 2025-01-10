#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>

#include "Shader.h"

class HealthBar {
private:
    float maxHealth;
    float currentHealth;

    bool alive = true;

    glm::vec2 position;
    glm::vec2 size;

    GLuint VAO, VBO;

    void initRenderData(); 

    void updateUniforms(Shader* shader);

public:
    HealthBar(float maxHealth, glm::vec2 position, glm::vec2 size);
    ~HealthBar();

    void update(float deltaTime);
    void render(Shader* shader);
    void setHealth(float health);
    void increaseHealth(float increaseRate);
    bool isAlive() const;
};

#endif
