#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/matrix_transform_2d.hpp"
#include "glm/gtx/transform.hpp"

namespace YingLong {

	struct Transform2DComponent
	{
		glm::vec2 Position = glm::vec2(0.0f, 0.0f);
		float RotateAngle = 0.0f;
		glm::vec2 Scale = glm::vec2(1.0f, 1.0f);

		glm::mat3 GetTransform()
		{
			glm::mat3 transform = glm::translate(glm::mat3(1.0f), Position);
			transform = glm::rotate(transform, RotateAngle);
			transform = glm::scale(transform, Scale);
			return transform;
		}

		glm::mat3 GetInverseTransform()
		{
			return glm::inverse(GetTransform());
		}
	};

	struct Transform3DComponent
	{
		glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 Forward = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::mat3 GetTransform()
		{
			return glm::lookAt(Position, Position + Forward, Up);
		}

		glm::mat3 GetInverseTransform()
		{
			return glm::inverse(GetTransform());
		}
	};

	struct Camera3DComponent
	{

	};
}