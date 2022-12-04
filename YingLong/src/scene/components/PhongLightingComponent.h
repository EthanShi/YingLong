#pragma once

#include "glm/glm.hpp"

#include "renderer/Renderer3D.h"
#include "renderer/PhongLighting/PhongMaterial.h"

namespace YingLong {

	struct PhongLightComponent
	{
		PhongLightComponent(const glm::vec3& Ambient, const glm::vec3& Diffuse, const glm::vec3& Specular)
			: m_Ambient(Ambient)
			, m_Diffuse(Diffuse)
			, m_Specular(Specular)
		{}

		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
	};

	struct PhongMaterialComponent
	{
		PhongMaterialComponent(const PhongMaterial& Material)
			: m_Material(Material)
		{}

		PhongMaterialComponent(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular, float Shininess)
			: m_Material(Ambient, Diffuse, Specular, Shininess)
		{}

		PhongMaterial m_Material;
	};
}

