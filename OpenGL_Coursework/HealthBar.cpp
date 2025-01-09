#include "HealthBar.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Шейдеры
const char* vertexShaderCode = R"(
    #version 440 core
    layout (location = 0) in vec2 aPos;

    uniform mat4 projection;
    uniform vec2 offset;
    uniform vec2 scale;

    void main() {
        vec2 scaledPos = aPos * scale + offset;
        gl_Position = projection * vec4(scaledPos, 0.0, 1.0);
    }
)";

const char* fragmentShaderCode = R"(
    #version 440 core
    out vec4 FragColor;
    uniform vec3 color;

    void main() {
        FragColor = vec4(color, 1.0);
    }
)";

HealthBar::HealthBar(float maxHealth, glm::vec2 position, glm::vec2 size)
    : maxHealth(maxHealth), currentHealth(maxHealth), position(position), size(size) {
    initRenderData();
    shaderProgram = compileShader(vertexShaderCode, fragmentShaderCode);
}

HealthBar::~HealthBar() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
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

GLuint HealthBar::compileShader(const std::string& vertexCode, const std::string& fragmentCode) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexSource = vertexCode.c_str();
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentSource = fragmentCode.c_str();
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
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

void HealthBar::render() {
    glUseProgram(shaderProgram);

    glm::vec2 scale(size.x, size.y);
    scale.x *= (currentHealth / maxHealth);

    GLint offsetLoc = glGetUniformLocation(shaderProgram, "offset");
    glUniform2f(offsetLoc, position.x, position.y);

    GLint scaleLoc = glGetUniformLocation(shaderProgram, "scale");
    glUniform2f(scaleLoc, scale.x, scale.y);

    GLint colorLoc = glGetUniformLocation(shaderProgram, "color");
    if(currentHealth >= 50.0f)
        glUniform3f(colorLoc, 0.0f, 1.0f, 0.0f); 
    else if (currentHealth >= 30.0f)
        glUniform3f(colorLoc, 0.6f, 0.6f, 0.0f);
    else
        glUniform3f(colorLoc, 0.5f, 0.0f, 0.0f);


    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}
