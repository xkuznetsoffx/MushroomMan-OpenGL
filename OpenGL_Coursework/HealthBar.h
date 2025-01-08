#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>

class HealthBar {
private:
    float maxHealth;
    float currentHealth;

    bool alive = true;

    glm::vec2 position;
    glm::vec2 size;

    GLuint VAO, VBO;

    void initRenderData(); 
    GLuint compileShader(const std::string& vertexCode, const std::string& fragmentCode);

public:
    HealthBar(float maxHealth, glm::vec2 position, glm::vec2 size);
    ~HealthBar();
    GLuint shaderProgram; 

    void update(float deltaTime);
    void render();
    void setHealth(float health);
    void increaseHealth(float increaseRate);
    bool isAlive() const;
};

#endif
