#include "Light.h"

unsigned PointLight::pointLightCnt = 0;

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
	ambient(ambient), diffuse(diffuse), specular(specular)
{
}

DirectionLight::DirectionLight(
	glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
	glm::vec3 direction
) 
	: Light(ambient, diffuse, specular), direction(direction)
{
}

void DirectionLight::sendToShader(Shader* shader)
{
	shader->setVec3("dirLight.direction", direction);
	shader->setVec3("dirLight.ambient", ambient);
	shader->setVec3("dirLight.diffuse", diffuse);
	shader->setVec3("dirLight.specular", specular);
}

PointLight::PointLight(
	glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
	glm::vec3 position, float constant, float linear, float quadratic
) 
	: Light(ambient, diffuse, specular), position(position), constant(constant), linear(linear), quadratic(quadratic)
{
	index = pointLightCnt;
	++pointLightCnt;
}

void PointLight::sendToShader(Shader* shader)
{
	std::string index = std::to_string(this->index);
	shader->setVec3("pointLights[" + index + "].position", position);
	shader->setVec3("pointLights[" + index + "].ambient", ambient);
	shader->setVec3("pointLights[" + index + "].diffuse", diffuse);
	shader->setVec3("pointLights[" + index + "].specular", specular);
	shader->setFloat("pointLights[" + index + "].constant", constant);
	shader->setFloat("pointLights[" + index + "].linear", linear);
	shader->setFloat("pointLights[" + index + "].quadratic", quadratic);
}

SpotLight::SpotLight(
	glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
	glm::vec3 position, glm::vec3 direction, float constant, float linear, float quadratic,
	float radCutOff, float radOuterCutOff
)
	: PointLight(ambient, diffuse, specular, position, constant, linear, quadratic), 
	direction(direction), RADIUS_CUT_OFF(radCutOff), RADIUS_OUTER_CUT_OFF(radOuterCutOff)
{
}

void SpotLight::sendToShader(Shader* shader)
{
	shader->setVec3("spotLight.position", position);
	shader->setVec3("spotLight.direction", direction);
	shader->setVec3("spotLight.ambient", ambient);
	shader->setVec3("spotLight.diffuse", diffuse);
	shader->setVec3("spotLight.specular", specular);
	shader->setFloat("spotLight.constant", constant);
	shader->setFloat("spotLight.linear", linear);
	shader->setFloat("spotLight.quadratic", quadratic);

	shader->setFloat("spotLight.cutOff", glm::cos(glm::radians(radCutOff)));
	shader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(radOuterCutOff)));
}

void SpotLight::updatePosition(glm::vec3 position)
{
	this->position = position;
}

void SpotLight::updateDirection(glm::vec3 direction)
{
	this->direction = direction;
}

bool SpotLight::isOn()
{
	return isFlashlightOn;
}

void SpotLight::turnOn()
{
	radCutOff = RADIUS_CUT_OFF;
	radOuterCutOff = RADIUS_OUTER_CUT_OFF;
	isFlashlightOn = true;
}

void SpotLight::turnOff()
{
	radCutOff = 0;
	radOuterCutOff = 0;
	isFlashlightOn = false;
}
