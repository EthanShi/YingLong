#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/matrix_transform_2d.hpp"
#include "glm/gtx/transform.hpp"

#include "renderer/Camera3D.h"
#include "utils/Property.h"

namespace YingLong {

	struct Transform3DComponent
	{
		glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 Scale = glm::vec3(1.f, 1.f, 1.f);
		Property<glm::vec3> Forward
		{
			glm::vec3(0.0f, 0.0f, -1.0f),
			[](const glm::vec3& NewValue, glm::vec3& InnerValue) { InnerValue = glm::normalize(NewValue); }
		};

		Property<glm::vec3> Up
		{
			glm::vec3(0.0f, 1.0f, 0.0f),
			[](const glm::vec3& NewValue, glm::vec3& InnerValue) { InnerValue = glm::normalize(NewValue); }
		};

		glm::mat4 GetTransform() const
		{
			return glm::lookAt(Position, Position + Forward.Value(), Up.Value()) * glm::scale(Scale);
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