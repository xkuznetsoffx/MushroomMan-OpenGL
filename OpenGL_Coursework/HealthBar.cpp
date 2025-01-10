#include "HealthBar.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

HealthBar::HealthBar(float maxHealth, glm::vec2 position, glm::vec2 size)
    : maxHealth(maxHealth), currentHealth(maxHealth), position(position), size(size) {
    initRenderData();
}

HealthBar::~HealthBar() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void HealthBar::initRenderData() {
    float vertices[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    GLuint indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void HealthBar::updateUniforms(Shader* shader)
{
    glm::vec2 scale(size.x, size.y);
    scale.x *= (currentHealth / maxHealth);

    shader->setVec2("offset", position.x, position.y);
    shader->setVec2("scale", scale.x, scale.y);
  
    if (currentHealth >= 50.0f)
        shader->setVec3("color", 0.0f, 1.0f, 0.0f);
    else if (currentHealth >= 30.0f)
        shader->setVec3("color", 0.6f, 0.6f, 0.0f);
    else
        shader->setVec3("color", 0.5f, 0.0f, 0.0f);
}

void HealthBar::update(float deltaTime) {
    currentHealth -= 3.5f * deltaTime;
    if (currentHealth < 0.0f) {
        currentHealth = 0.0f;
        alive = false;
    }
}

void HealthBar::setHealth(float health) {
    if (health < 0.0f) health = 0.0f;
    if (health > maxHealth) health = maxHealth;
    currentHealth = health;
}

void HealthBar::increaseHealth(float increaseRate)
{
    currentHealth += increaseRate;
    if (currentHealth > maxHealth)
        currentHealth = maxHealth;
}

bool HealthBar::isAlive() const
{
    return alive;
}

void HealthBar::render(Shader* shader) {
    shader->Use();

    updateUniforms(shader);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}
