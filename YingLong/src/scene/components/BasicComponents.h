#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/matrix_transform_2d.hpp"
#include "glm/gtx/transform.hpp"

#include "renderer/Camera3D.h"

namespace YingLong {

	struct Transform3DComponent
	{
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f);

		glm::mat4 GetTransform() const
		{
			return glm::lookAt(position, position + forward, up) * glm::scale(scale);
		}

		glm::mat4 GetInverseTransform() const
		{
			return glm::inverse(GetTransform());
		}
	};

	struct Camera3DComponent
	{
		Camera3D Camera;

		operator Camera3D() { return Camera; }
	};
}