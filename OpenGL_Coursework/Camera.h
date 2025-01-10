#pragma once

#include <chrono>

#include "Collision.h"
#include "Terrain.h"
#include "Sound.h"

#include <GL/glew.h>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

constexpr GLfloat YAW = 70.0f;
constexpr GLfloat PITCH = 0.0f;
constexpr GLfloat SPEED = 3.0f;
constexpr GLfloat SENSITIVTY = 0.05f;
constexpr GLfloat ZOOM = 45.0f;

class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        GLfloat yaw = YAW, GLfloat pitch = PITCH);

    Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
        GLfloat upX, GLfloat upY, GLfloat upZ, 
        GLfloat yaw, GLfloat pitch);

    ~Camera();

    glm::mat4 GetViewMatrix();
    glm::vec3 GetFront();
    glm::vec3 GetPoistion();
    GLfloat GetZoom();
    const AABB& getHitbox();

    void SetPosition(glm::vec3 position);
    void move(const glm::vec3 position);

    void updateCameraSpeed(float speed, float duration);
    void update();

    void ProcessKeyboard(const std::vector<Camera_Movement>& directions, GLfloat deltaTime, Terrain& terrain);
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(GLfloat yoffset);

private:
   

    void updateCameraVectors();

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Eular Angles
    GLfloat Yaw;
    GLfloat Pitch;
    // Camera options
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    GLfloat Zoom;
    GLfloat shakeAmplitude = 0.025f;
    GLfloat shakeFrequency = 15.0f;
    GLfloat shakeTime = 0.0f;

    AABB hitbox;

    Sound* steps;

    std::chrono::steady_clock::time_point speedBoostTime;
    bool speedBoostActivated = false;
};

