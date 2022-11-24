#pragma once

#include "glm/glm.hpp"

#include "renderer/Renderer3D.h"

using namespace YingLong;

struct PhongLightComponent
{
	PhongLightComponent(float Ambient, float Specular, const glm::vec3& Color)
		: m_AmbientStrength(Ambient)
		, m_SpecularStrength(Specular)
		, m_LightColor(Color)
	{}

	float m_AmbientStrength;
	float m_SpecularStrength;
	glm::vec3 m_LightColor;
};