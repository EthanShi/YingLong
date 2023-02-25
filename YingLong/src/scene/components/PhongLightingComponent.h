#pragma once

#include "glm/glm.hpp"

#include "renderer/Renderer3D.h"
#include "renderer/PhongLighting/PhongMaterial.h"
#include "renderer/PhongLighting/PhongLightingType.h"

namespace YingLong {

	struct PhongLightComponent
	{
		PhongLightComponent(PhongLightingType Type, const glm::vec3& Ambient, const glm::vec3& Diffuse, const glm::vec3& Specular)
			: Type(Type)
			, m_Ambient(Ambient)
			, m_Diffuse(Diffuse)
			, m_Specular(Specular)
		{}

		PhongLightingType Type;

		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
	};

	struct PhongDirectionalLightComponent : public PhongLightComponent
	{
		PhongDirectionalLightComponent(const glm::vec3& Ambient, const glm::vec3& Diffuse, const glm::vec3& Specular)
			: PhongLightComponent(PhongLightingType::Directional, Ambient, Diffuse, Specular)
		{}
	};

	struct PhongPointLightComponent : public PhongLightComponent
	{
		PhongPointLightComponent(const glm::vec3& Ambient, const glm::vec3& Diffuse, const glm::vec3& Specular,
			const float Constant, const float Linear, const float Quadratic)
			: PhongLightComponent(PhongLightingType::Point, Ambient, Diffuse, Specular)
			, m_Constant(Constant)
			, m_Linear(Linear)
			, m_Quadratic(Quadratic)
		{}

		float m_Constant;
		float m_Linear;
		float m_Quadratic;
	};

	struct PhongSpotLightComponent : public PhongPointLightComponent
	{
		PhongSpotLightComponent(const glm::vec3& Ambient, const glm::vec3& Diffuse, const glm::vec3& Specular,
			const float Constant, const float Linear, const float Quadratic,
			const glm::vec3& SpotDirection, const float InnerCutOffInCos, const float OuterCutOffInCos)
			: PhongPointLightComponent(Ambient, Diffuse, Specular, Constant, Linear, Quadratic)
			, m_SpotDirection(SpotDirection)
			, m_InnerCutOffInCos(InnerCutOffInCos)
			, m_OuterCutOffInCos(OuterCutOffInCos)
		{
			Type = PhongLightingType::Spot;
		}

		glm::vec3 m_SpotDirection;
		float m_InnerCutOffInCos;
		float m_OuterCutOffInCos;
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

