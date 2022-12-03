#pragma once

#include "glm/glm.hpp"

namespace YingLong {

	struct PhongMaterial
	{
		PhongMaterial() = default;
		PhongMaterial(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular, float Shininess)
			: Ambient(Ambient)
			, Diffuse(Diffuse)
			, Specular(Specular)
			, Shininess(Shininess)
		{}

		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
		float Shininess;
	};

}