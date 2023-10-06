#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/matrix_transform_2d.hpp"
#include "glm/gtx/transform.hpp"

#include "renderer/Camera3D.h"

namespace YingLong {

	struct Transform3DComponent
	{
	private:
		bool DirtyTransform = false;
		glm::mat4 Transform{ 1.f };
		glm::mat4 InverseTransform{ 1.f };
		glm::vec3 Position{ 0.f, 0.f, 0.f };
		glm::vec3 Scale{ 1.f, 1.f, 1.f };
		glm::vec3 Forward{ 0.f, 0.f, -1.f };
		glm::vec3 Up{ 0.f, 1.f, 0.f };

	public:
		void UpdateTransforms()
		{
			if (DirtyTransform)
			{
				DirtyTransform = false;
				InverseTransform = glm::lookAt(Position, Position + Forward, Up);
				Transform = glm::inverse(InverseTransform);
				InverseTransform = glm::scale(InverseTransform, Scale);
				Transform = glm::scale(Transform, Scale);
			}
		}

		glm::mat4 GetViewMatrix() {
			UpdateTransforms();
			return InverseTransform;
		}

		glm::mat4 GetTransform() {
			UpdateTransforms();
			return Transform;
		};
		glm::mat4 GetInverseTransform() {
			UpdateTransforms();
			return InverseTransform;
		};

		void SetPosition(const glm::vec3& Value) {
			if (Value != Position) {
				Position = Value;
				DirtyTransform = true;
			}
		}
		glm::vec3 GetPosition() const { return Position; }

		void SetScale(const glm::vec3& Value) {
			if (Value != Scale)
			{
				Scale = Value;
				DirtyTransform = true;
			}
		}
		glm::vec3 GetScale() const { return Scale; }

		void SetForward(const glm::vec3& Value) {
			if (Value != Forward)
			{
				Forward = Value;
				DirtyTransform = true;
			}
		}
		glm::vec3 GetForward() const { return Forward; }

		void SetUp(const glm::vec3& Value) {
			if (Value != Up)
			{
				Up = Value;
				DirtyTransform = true;
			}
		}
		glm::vec3 GetUp() const { return Up; }

		void Move(const glm::vec3& Translate)
		{
			if (!DOUBLE_EQUAL(glm::length(Translate), 0.f))
			{
				Position += Translate;
				DirtyTransform = true;
			}
		}

		void Rotate(float Pitch, float Yaw, float Roll)
		{
			if (DOUBLE_EQUAL(Pitch, 0.f) && DOUBLE_EQUAL(Yaw, 0.f) && DOUBLE_EQUAL(Roll, 0.f)) return;

			glm::mat4 rotMat(1.f);
			rotMat = glm::rotate(rotMat, glm::radians(Pitch), glm::cross(Forward, Up));
			rotMat = glm::rotate(rotMat, glm::radians(Yaw), Up);
			rotMat = glm::rotate(rotMat, glm::radians(Roll), Forward);
			Forward = glm::vec3(rotMat * glm::vec4(Forward, 0.f));
			Up = glm::vec3(rotMat * glm::vec4(Up, 0.f));
			DirtyTransform = true;
		}
	};

	struct Camera3DComponent
	{
		Camera3D Camera;

		operator Camera3D() { return Camera; }
	};

	struct FreeMovementComponent
	{
		float MoveSpeed = 400.f;
		float TurnRate = 5.0f;
	};
}