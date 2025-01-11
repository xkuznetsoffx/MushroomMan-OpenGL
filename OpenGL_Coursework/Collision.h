#pragma once

#include "glm/glm.hpp"

struct AABB
{
	glm::vec3 min = glm::vec3(FLT_MAX);
	glm::vec3 max = glm::vec3(-FLT_MAX);
};

bool checkCollision(const AABB& box1, const AABB& box2);