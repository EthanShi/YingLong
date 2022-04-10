
#include "YingLongPCH.h"



#include "Camera3D.h"


namespace YingLong {

	Camera3D::Camera3D(float fov, float aspect, float zNear, float zFar)
		:m_CameraInfo({ fov, aspect, zNear, zFar })
	{
		UpdatePerspective();
	}

	Camera3D::Camera3D(const Camera3DInfo& info)
		: m_CameraInfo(info)
	{
		UpdatePerspective();
	}

	void Camera3D::SetFov(float fov)
	{
		if (fov < 0 && fov < 180)
		{
			m_CameraInfo.fov = fov;
			UpdatePerspective();
		}
	}

	void Camera3D::SetAspect(float aspect)
	{
		m_CameraInfo.aspect = aspect;
		UpdatePerspective();
	}

	//void Camera3D::Rotate(float angle, const glm::vec3& axis)
	//{
	//	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), angle, axis);
	//	m_CameraInfo.forward = rotateMat * glm::vec4(m_CameraInfo.forward, 0);
	//	m_CameraInfo.up = rotateMat * glm::vec4(m_CameraInfo.up, 0);
	//	UpdateCacheInfos();
	//}

	glm::mat4 Camera3D::GetPerspective() const
	{
		return m_Perspective;
	}

	void Camera3D::UpdatePerspective()
	{
		m_Perspective = glm::perspective(glm::radians(m_CameraInfo.fov), m_CameraInfo.aspect, m_CameraInfo.zNear, m_CameraInfo.zFar);
	}

}