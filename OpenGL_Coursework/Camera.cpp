#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM), Position(position), WorldUp(up), 
Yaw(yaw), Pitch(pitch) {
	hitbox.min = Position - glm::vec3(0.1f);
	hitbox.max = Position + glm::vec3(0.1f);
	initSounds();
	updateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
	GLfloat upX, GLfloat upY, GLfloat upZ,
	GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
	MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM),
	Yaw(yaw), Pitch(pitch) 
{
	Position = glm::vec3(posX, posY, posZ);
	hitbox.min = Position - glm::vec3(0.2f, 0.5f, 0.2f);
	hitbox.max = Position + glm::vec3(0.2f, 0.5f, 0.2f);
	WorldUp = glm::vec3(upX, upY, upZ);
	initSounds();
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(Position, Position + Front, Up);
}

glm::vec3 Camera::GetFront()
{
	return Front;
}

glm::vec3 Camera::GetPoistion()
{
	return Position;
}

GLfloat Camera::GetZoom(){
	return Zoom;
}

const AABB& Camera::getHitbox()
{
	return hitbox;
}

void Camera::SetPosition(glm::vec3 position)
{
	this->Position = position;
	hitbox.min = Position - glm::vec3(0.2f, 0.5f, 0.2f);
	hitbox.max = Position + glm::vec3(0.2f, 0.5f, 0.2f);
}

void Camera::move(const glm::vec3 position)
{
	this->Position += position;
	hitbox.min = Position - glm::vec3(0.2f, 0.5f, 0.2f);
	hitbox.max = Position + glm::vec3(0.2f, 0.5f, 0.2f);
}

void Camera::updateCameraSpeed(float speed, float duration)
{
	if (!speedBoostActivated)
	{
		MovementSpeed += speed;
		speedBoostTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(static_cast<int>(duration * 1000));
	}
	else
		speedBoostTime += std::chrono::milliseconds(static_cast<int>(duration * 1000));

	speedBoostActivated = true;
}

void Camera::update()
{
	if (speedBoostActivated && std::chrono::steady_clock::now() >= speedBoostTime) {
		MovementSpeed = SPEED;
		speedBoostActivated = false;
	}
}

void Camera::ProcessKeyboard(const std::vector<Camera_Movement>& directions, GLfloat deltaTime, Terrain& terrain) {
	glm::vec3 movement(0.0f);
	GLfloat velocity = MovementSpeed * deltaTime;

	static float stepSoundTimer = 0.0f;
	stepSoundTimer += deltaTime;

	glm::vec3 FlatFront = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
	glm::vec3 FlatRight = glm::normalize(glm::vec3(Right.x, 0.0f, Right.z));

	for (const auto& direction : directions)
	{
		switch (direction) {
		case FORWARD:
			movement += FlatFront;
			break;
		case BACKWARD:
			movement -= FlatFront;
			break;
		case LEFT:
			movement -= FlatRight;
			break;
		case RIGHT:
			movement += FlatRight;
			break;
		}
	}
	if (glm::length(movement) > 0.0f &&
		terrain.isCoordInMap((Position + movement * velocity).x, (Position + movement * velocity).z)
		)
	{
		if (stepSoundTimer >= 1.5 / MovementSpeed) {
			static std::random_device rd;
			static std::mt19937 gen(rd());
			std::uniform_int_distribution<> distrib(0, stepSounds.size() - 1);

			int randomSoundIndex = distrib(gen);
			stepSounds[randomSoundIndex]->play();

			stepSoundTimer = 0.0f;
		}
		movement = glm::normalize(movement);
		Position += movement * velocity;
		shakeTime += deltaTime * shakeFrequency;
		GLfloat shakeOffset = sin(shakeTime) * shakeAmplitude;
		Position.y = terrain.getCurrentHeightFromMap(Position.x, Position.z) + 0.8f + shakeOffset;

		hitbox.min = Position - glm::vec3(0.1f, 0.5f, 0.1f);
		hitbox.max = Position + glm::vec3(0.1f, 0.5f, 0.1f);
	}	
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch) {
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yoffset) {
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));  
	Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::initSounds()
{
	for (size_t i = 1; i < 6; ++i) {
		std::string index = std::to_string(i);
		stepSounds.emplace_back(std::make_unique<Sound>("assets\\sounds\\sand"+ index +".wav"));
		stepSounds[i-1]->setVolume(0.1f);
	}
}
