#pragma once

#include "Shader.h"

class Light
{
public:
	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

	virtual void sendToShader(Shader* shader) = 0;

protected:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};	

class DirectionLight : public Light
{
public:
	DirectionLight(
		glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3 diffuse = glm::vec3(0.4f, 0.4f, 0.4f),
		glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f)
	);

	void sendToShader(Shader* shader) override;

private:
	glm::vec3 direction;
};

class PointLight : public Light
{
public:
	PointLight(
		glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f),
		glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3 position = glm::vec3(0.0f),
		float constant = 1.0f,
		float linear = 0.09f,
		float quadratic = 0.032f
	);

	void sendToShader(Shader* shader) override;

protected:
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;

private:
	unsigned index;
	static unsigned pointLightCnt;
};

class SpotLight : public PointLight
{
public:
	SpotLight(
		glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 direction = glm::vec3(0.0f),
		float constant = 1.0f,
		float linear = 0.09f,
		float quadratic = 0.032f,
		float radCutOff = 10.0f,
		float radOuterCutOff = 15.0f
	);

	void sendToShader(Shader* shader) override;

	void updatePosition(glm::vec3 position);
	void updateDirection(glm::vec3 direction);

	bool isOn();
	void turnOn();
	void turnOff();


private:
	glm::vec3 direction;

	const float RADIUS_CUT_OFF;
	const float RADIUS_OUTER_CUT_OFF;
	float radCutOff = 0.0f;
	float radOuterCutOff = 0.0f;
	bool isFlashlightOn = false;

};
