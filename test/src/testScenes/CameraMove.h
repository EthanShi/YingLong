#pragma once

#include "glm/glm.hpp"

#include "input/Input.h"
#include "scene/Scene.h"

#include "scene/systems/FreeMovementSystem.h"

using namespace YingLong;

class CameraMoveScene : public Scene
{
public:
	CameraMoveScene();
	virtual ~CameraMoveScene();

	virtual std::string GetName() override { return "CameraMoveScene"; }

protected:
	virtual void OnActive(const std::shared_ptr<Engine>& OwnerEngine, const std::shared_ptr<Scene>& This) override;
	virtual void Update(float Deltatime) override;
	virtual void DrawImgui(float Deltatime) override;

	virtual void CreateDefaultCamera() override;

private:
	FreeMovementSystem m_FreeMovementSystem;
};

