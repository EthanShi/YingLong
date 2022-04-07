
#include "YingLongPCH.h"



#include "Camera.h"


namespace YingLong {

	Camera::Camera(float fov, float aspect, float zNear, float zFar, const glm::vec3& pos, const glm::vec3& forward, const glm::vec3& up)
		:m_CameraInfo({ fov, aspect, zNear, zFar, pos, forward, up })
	{
		UpdateCacheInfos();
	}

	Camera::Camera(const CameraInfo& info)
		: m_CameraInfo(info)
	{
		UpdateCacheInfos();
	}

	void Camera::SetFov(float fov)
	{
		if (fov < 0 && fov < 180)
		{
			m_CameraInfo.fov = fov;
			UpdateCacheInfos();
		}
	}

	void Camera::SetAspect(float aspect)
	{
		m_CameraInfo.aspect = aspect;
		UpdateCacheInfos();
	}

	void Camera::SetPostion(const glm::vec3& pos)
	{
		m_CameraInfo.pos = pos;
		UpdateCacheInfos();
	}

	void Camera::SetUp(const glm::vec3& up)
	{
		m_CameraInfo.up = glm::normalize(up);
		UpdateCacheInfos();
	}

	void Camera::SetForward(const glm::vec3& forward)
	{
		m_CameraInfo.forward = glm::normalize(forward);
		UpdateCacheInfos();
	}

	void Camera::Move(float distance, const glm::vec3& forward)
	{
		if (forward == glm::vec3())
		{
			m_CameraInfo.pos += m_CameraInfo.forward * distance;
		}
		else
		{
			m_CameraInfo.pos += glm::normalize(forward) * distance;
		}
		UpdateCacheInfos();
	}

	void Camera::Rotate(float angle, const glm::vec3& axis)
	{
		glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), angle, axis);
		m_CameraInfo.forward = rotateMat * glm::vec4(m_CameraInfo.forward, 0);
		m_CameraInfo.up = rotateMat * glm::vec4(m_CameraInfo.up, 0);
		UpdateCacheInfos();
	}

	glm::mat4 Camera::GetPerspective() const
	{
		return m_Perspective;
	}

	glm::mat4 Camera::GetInverseTransform() const
	{
		return m_InverseTransform;
	}

	glm::mat4 Camera::GetTransform() const
	{
		return m_Transform;
	}

	glm::vec3 Camera::GetForward() const
	{
		return m_CameraInfo.forward;
	}

	glm::vec3 Camera::GetUp() const
	{
		return m_CameraInfo.up;
	}

	void Camera::UpdateCacheInfos()
	{
		m_Perspective = glm::perspective(glm::radians(m_CameraInfo.fov), m_CameraInfo.aspect, m_CameraInfo.zNear, m_CameraInfo.zFar);
		m_InverseTransform = glm::lookAt(m_CameraInfo.pos, m_CameraInfo.pos + m_CameraInfo.forward, m_CameraInfo.up);
		m_Transform = glm::inverse(m_InverseTransform);
	}

}