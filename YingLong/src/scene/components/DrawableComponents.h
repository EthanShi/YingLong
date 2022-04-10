#pragma once

#include "glm/glm.hpp"

#include "renderer/Texture.h"

using namespace YingLong;

struct SpriteComponent
{
	glm::vec4 color;
	Texture_SPtr Texture;
};