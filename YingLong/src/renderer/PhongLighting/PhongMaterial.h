#pragma once

#include "glm/glm.hpp"
#include "renderer/Texture.h"

namespace YingLong {

	class PhongMaterial
	{
	public:
		PhongMaterial() = default;
		PhongMaterial(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular, float Shininess)
			: m_Ambient(Ambient)
			, m_Diffuse(Diffuse)
			, m_Specular(Specular)
			, m_Shininess(Shininess)
		{}
		PhongMaterial(const std::string& DiffuseMapFileName, const std::string& SpecularMapFileName, glm::vec3 Specular, float Shininess);

		Texture& GetDiffuseMap();
		Texture& GetSpecularMap();

		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
		float m_Shininess;

	private:
		uint32 m_DiffuseMapTextureID;
		uint32 m_SpecularMapTextureID;
	};

}