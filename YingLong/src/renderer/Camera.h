#pragma once

#include "glm/glm.hpp"

#include "core/Macros.h"

namespace YingLong {

	struct YINGLONG_API CameraInfo
	{
		float fov = 45.0f;
		float aspect = 16.0f / 9.0f;
		float zNear = 0.1f;
		float zFar = 100.0f;
		glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	};

	class YINGLONG_API Camera
	{
	private:
		CameraInfo m_CameraInfo;

		glm::mat4 m_Perspective = glm::mat4();
		glm::mat4 m_Transform = glm::mat4();
		glm::mat4 m_InverseTransform = glm::mat4();

	public:
		Camera() {}
		Camera(float fov, float aspect, float zNear, float zFar, const glm::vec3& pos, const glm::vec3& front, const glm::vec3& up);
		Camera(const CameraInfo& info);
		~Camera() {}

		// Set functions
		void SetFov(float fov);
		void SetAspect(float aspect);
		void SetPostion(const glm::vec3& pos);
		void SetUp(const glm::vec3& up);
		void SetForward(const glm::vec3& forward);

		// Move functions
		void Move(float distance, const glm::vec3& forward = glm::vec3());
		void Rotate(float angle, const glm::vec3& axis);

		// Get functions
		glm::mat4 GetPerspective() const;
		glm::mat4 GetInverseTransform() const;
		glm::mat4 GetTransform() const;
		glm::vec3 GetForward() const;
		glm::vec3 GetUp() const;

	private:
		void UpdateCacheInfos();
	};

}