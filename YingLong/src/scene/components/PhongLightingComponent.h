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
			, Ambient(Ambient)
			, Diffuse(Diffuse)
			, Specular(Specular)
		{}

		PhongLightingType Type;

		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
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
			, Constant(Constant)
			, Linear(Linear)
			, Quadratic(Quadratic)
		{}

		float Constant;
		float Linear;
		float Quadratic;
	};

	struct PhongSpotLightComponent : public PhongPointLightComponent
	{
		PhongSpotLightComponent(const glm::vec3& Ambient, const glm::vec3& Diffuse, const glm::vec3& Specular,
			const float Constant, const float Linear, const float Quadratic,
			const float InnerCutOffInCos, const float OuterCutOffInCos)
			: PhongPointLightComponent(Ambient, Diffuse, Specular, Constant, Linear, Quadratic)
			, InnerCutOffInCos(InnerCutOffInCos)
			, OuterCutOffInCos(OuterCutOffInCos)
		{
			Type = PhongLightingType::Spot;
		}

		float InnerCutOffInCos;
		float OuterCutOffInCos;
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

