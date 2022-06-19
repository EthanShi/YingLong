#pragma once

#include "glm/glm.hpp"

#include "scene/Scene.h"
#include "utils/Property.h"

using namespace YingLong;

class CameraMoveScene : public Scene
{
public:
	CameraMoveScene();
	virtual ~CameraMoveScene() override {}

	virtual void Update(float Deltatime) override;
	virtual void DrawImgui(float Deltatime) override;

private:
	float m_MoveForwardValue = 0.f;
	float m_MoveRightValue = 0.f;
	float m_MoveSpeed = 4.f;

private:
	void MoveCameraForward(float Value);
	void MoveCameraRight(float Value);
};

