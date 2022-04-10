#pragma once

#include "glm/glm.hpp"

#include "core/Macros.h"

namespace YingLong {

	struct YINGLONG_API Camera3DInfo
	{
		float fov = 45.0f;
		float aspect = 16.0f / 9.0f;
		float zNear = 0.1f;
		float zFar = 100.0f;
	};

	class YINGLONG_API Camera3D
	{
	private:
		Camera3DInfo m_CameraInfo;

		glm::mat4 m_Perspective = glm::mat4();

	public:
		Camera3D() {}
		Camera3D(float fov, float aspect, float zNear, float zFar);
		Camera3D(const Camera3DInfo& info);
		~Camera3D() {}

		// Set functions
		void SetFov(float fov);
		void SetAspect(float aspect);

		// Get functions
		glm::mat4 GetPerspective() const;

	private:
		void UpdatePerspective();
	};

}