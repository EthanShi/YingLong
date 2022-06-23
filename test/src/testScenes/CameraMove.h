#pragma once

#include "glm/glm.hpp"

#include "input/InputTypes.h"
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

	InputCallbackHandler CallbackHandlers[9];

private:
	void MoveCameraForward(float Value);
	void MoveCameraRight(float Value);
	void TurnCamera(const glm::dvec2& OldPos, const glm::dvec2& NewPos);
};

