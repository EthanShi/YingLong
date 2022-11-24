#pragma once

#include "scene/Scene.h"

#include "CameraMove.h"

#include "scene/systems/DrawBasicLightingSystem.h"

using namespace YingLong;

class BasicLightingScene : public CameraMoveScene
{
public:
	BasicLightingScene();
	virtual ~BasicLightingScene() {}

	virtual std::string GetName() override { return "BasicLightingScene"; }

protected:
	virtual void OnActive(const std::shared_ptr<Engine>& OwnerEngine, const std::shared_ptr<Scene>& This) override;
	virtual void Update(float Deltatime) override;
	virtual void DrawEntities(float Deltatime) override;

	virtual std::string GetShaderPath() override;

private:
	void CreatePhongLight();

private:
	DrawBasicLightingSystem m_DrawBasicLightingSystem;
};