#pragma once

#include "glm/glm.hpp"

struct Transform2D
{
	glm::mat3 Transform;

	operator glm::mat3() { return Transform; }
};

struct Transform3D
{
	glm::mat4 Transform;

	operator glm::mat4() { return Transform; }
};