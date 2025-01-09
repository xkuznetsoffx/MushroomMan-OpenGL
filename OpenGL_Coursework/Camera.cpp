#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM), Position(position), WorldUp(up), 
Yaw(yaw), Pitch(pitch) {
	hitbox.min = Position - glm::vec3(0.1f);
	hitbox.max = Position + glm::vec3(0.1f);
	updateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
	GLfloat upX, GLfloat upY, GLfloat upZ,
	GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
	MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM),
	Yaw(yaw), Pitch(pitch) 
{
	Position = glm::vec3(posX, posY, posZ);
	hitbox.min = Position - glm::vec3(0.1f, 0.5f, 0.1f);
	hitbox.max = Position + glm::vec3(0.1f, 0.5f, 0.1f);
	WorldUp = glm::vec3(upX, upY, upZ);
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
	hitbox.min = Position - glm::vec3(0.1f, 0.5f, 0.1f);
	hitbox.max = Position + glm::vec3(0.1f, 0.5f, 0.1f);
}

void Camera::move(glm::vec3 position)
{
	this->Position += position;
	hitbox.min = Position - glm::vec3(0.1f, 0.5f, 0.1f);
	hitbox.max = Position + glm::vec3(0.1f, 0.5f, 0.1f);
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime, Terrain& terrain) {
	GLfloat velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD &&
		terrain.isCoordInMap((Position+Front*velocity).x,
			(Position+Front * velocity).z)
		)
		Position += Front * velocity;
	if (direction == BACKWARD &&
		terrain.isCoordInMap((Position - Front * velocity).x,
			(Position - Front * velocity).z)
		)
		Position -= Front * velocity;
	if (direction == LEFT &&
		terrain.isCoordInMap((Position - Right * velocity).x,
			(Position - Right * velocity).z)
		)
		Position -= Right * velocity;
	if (direction == RIGHT &&
		terrain.isCoordInMap((Position + Right * velocity).x,
			(Position + Right * velocity).z))
		Position += Right * velocity;

	shakeTime += deltaTime * shakeFrequency;
	GLfloat shakeOffset = sin(shakeTime) * shakeAmplitude;
	Position.y = terrain.getCurrentHeightFromMap(Position.x, Position.z) + 0.8f + shakeOffset;

	hitbox.min = Position - glm::vec3(0.1f, 0.5f, 0.1f);
	hitbox.max = Position + glm::vec3(0.1f, 0.5f, 0.1f);
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
