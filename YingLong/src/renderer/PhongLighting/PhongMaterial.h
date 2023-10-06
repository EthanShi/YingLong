#pragma once

#include "glm/glm.hpp"
#include "renderer/Texture.h"

namespace YingLong {

	class PhongMaterial
	{
	public:
		PhongMaterial() = default;
		PhongMaterial(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular, float Shininess)
			: Ambient(Ambient)
			, Diffuse(Diffuse)
			, Specular(Specular)
			, Shininess(Shininess)
		{}
		PhongMaterial(const std::string& DiffuseMapFileName, const std::string& SpecularMapFileName, glm::vec3 Specular, float Shininess);

		Texture& GetDiffuseMap();
		Texture& GetSpecularMap();

		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
		float Shininess;

	private:
		uint32 DiffuseMapTextureID;
		uint32 SpecularMapTextureID;
	};

}