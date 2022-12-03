#pragma once

#include "glm/glm.hpp"

#include "renderer/Renderer3D.h"
#include "renderer/PhongLighting/PhongMaterial.h"

namespace YingLong {

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

	struct PhongMaterialComponent
	{
		PhongMaterialComponent(const PhongMaterial& Material)
			: Material(Material)
		{}

		PhongMaterialComponent(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular, float Shininess)
			: Material(Ambient, Diffuse, Specular, Shininess)
		{}

		PhongMaterial Material;
	};
}

