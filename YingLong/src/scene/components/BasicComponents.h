#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/matrix_transform_2d.hpp"
#include "glm/gtx/transform.hpp"

#include "renderer/Camera3D.h"

namespace YingLong {

	struct Transform3DComponent
	{
	private:
		bool m_DirtyTransform = false;
		glm::mat4 m_Transform{ 1.f };
		glm::vec3 m_Position{ 0.f, 0.f, 0.f };
		glm::vec3 m_Scale{ 1.f, 1.f, 1.f };
		glm::vec3 m_Forward{ 0.f, 0.f, -1.f };
		glm::vec3 m_Up{ 0.f, 1.f, 0.f };

	public:
		glm::mat4 GetTransform() {
			if (m_DirtyTransform)
			{
				m_DirtyTransform = false;
				m_Transform = glm::lookAt(m_Position, m_Position + m_Forward, m_Up) * glm::scale(m_Scale);
			}
			return m_Transform; 
		};
		glm::mat4 GetInverseTransform() { return glm::inverse(GetTransform()); };

		void SetPosition(const glm::vec3& Value) {
			if (Value != m_Position) {
				m_Position = Value;
				m_DirtyTransform = true;
			}
		}
		glm::vec3 GetPosition() const { return m_Position; }

		void SetScale(const glm::vec3& Value) {
			if (Value != m_Scale)
			{
				m_Scale = Value;
				m_DirtyTransform = true;
			}
		}
		glm::vec3 GetScale() const { return m_Scale; }

		void SetForward(const glm::vec3& Value) {
			if (Value != m_Forward)
			{
				m_Forward = Value;
				m_DirtyTransform = true;
			}
		}
		glm::vec3 GetForward() const { return m_Forward; }

		void SetUp(const glm::vec3& Value) {
			if (Value != m_Up)
			{
				m_Up = Value;
				m_DirtyTransform = true;
			}
		}
		glm::vec3 GetUp() const { return m_Up; }

		void Move(const glm::vec3& Translate)
		{
			if (!DOUBLE_EQUAL(glm::length(Translate), 0.f))
			{
				m_Position += Translate;
				m_DirtyTransform = true;
			}
		}

		void Rotate(float Pitch, float Yaw, float Roll)
		{
			if (DOUBLE_EQUAL(Pitch, 0.f) && DOUBLE_EQUAL(Yaw, 0.f) && DOUBLE_EQUAL(Roll, 0.f)) return;

			glm::mat4 rotMat(1.f);
			rotMat = glm::rotate(rotMat, glm::radians(Pitch), glm::cross(m_Forward, m_Up));
			rotMat = glm::rotate(rotMat, glm::radians(Yaw), m_Up);
			rotMat = glm::rotate(rotMat, glm::radians(Roll), m_Forward);
			m_Forward = glm::vec3(rotMat * glm::vec4(m_Forward, 0.f));
			m_Up = glm::vec3(rotMat * glm::vec4(m_Up, 0.f));
			m_DirtyTransform = true;
		}
	};

	struct Camera3DComponent
	{
		Camera3D Camera;

		operator Camera3D() { return Camera; }
	};

	struct DefaultMovementComponent
	{

	};
}