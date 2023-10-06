
#include "YingLongPCH.h"



#include "Camera3D.h"


namespace YingLong {

	Camera3D::Camera3D(float fov, float aspect, float zNear, float zFar)
		:CameraInfo({ fov, aspect, zNear, zFar })
	{
		UpdatePerspective();
	}

	Camera3D::Camera3D(const Camera3DInfo& info)
		: CameraInfo(info)
	{
		UpdatePerspective();
	}

	void Camera3D::SetFov(float fov)
	{
		if (fov < 0 && fov < 180)
		{
			CameraInfo.fov = fov;
			UpdatePerspective();
		}
	}

	void Camera3D::SetAspect(float aspect)
	{
		CameraInfo.aspect = aspect;
		UpdatePerspective();
	}

	//void Camera3D::Rotate(float angle, const glm::vec3& axis)
	//{
	//	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), angle, axis);
	//	CameraInfo.forward = rotateMat * glm::vec4(CameraInfo.forward, 0);
	//	CameraInfo.up = rotateMat * glm::vec4(CameraInfo.up, 0);
	//	UpdateCacheInfos();
	//}

	glm::mat4 Camera3D::GetPerspective() const
	{
		return Perspective;
	}

	void Camera3D::UpdatePerspective()
	{
		Perspective = glm::perspective(glm::radians(CameraInfo.fov), CameraInfo.aspect, CameraInfo.zNear, CameraInfo.zFar);
	}

}