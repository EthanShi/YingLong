#pragma once

#include "scene/Scene.h"
#include "renderer/Mesh.h"
#include "scene/systems/FreeMovementSystem.h"
#include "scene/systems/DrawBasic3DMeshSystem.h"

using namespace YingLong;

class DepthTestScene : public Scene
{
public:
	DepthTestScene();
	virtual ~DepthTestScene() override {}

	virtual std::string GetName() override { return "DepthTestScene"; }

protected:
	virtual void OnActive(const std::shared_ptr<Engine>& OwnerEngine, const std::shared_ptr<Scene>& This) override;
	virtual void Update(float DeltaTime) override;
	virtual void DrawEntities(float deltatime) override;

	virtual void CreateDefaultCamera() override;

private:
	Mesh m_CubeMesh;
	FreeMovementSystem m_FreeMovementSystem;
	DrawBasic3DMeshSystem m_DrawBasic3DMeshSystem;
};

