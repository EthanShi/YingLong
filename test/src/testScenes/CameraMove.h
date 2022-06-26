#pragma once

#include "glm/glm.hpp"

#include "input/InputTypes.h"
#include "scene/Scene.h"

#include "scene/systems/FreeMovementSystem.h"

using namespace YingLong;

class CameraMoveScene : public Scene
{
public:
	CameraMoveScene();
	virtual ~CameraMoveScene();

	virtual void Update(float Deltatime) override;
	virtual void DrawImgui(float Deltatime) override;

protected:
	virtual void CreateDefaultCamera() override;

private:
	InputCallbackHandler CallbackHandlers[10];

	FreeMovementSystem m_FreeMovementSystem;
};

