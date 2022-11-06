#pragma once

#include "scene/Scene.h"
#include "renderer/Mesh.h"
#include "scene/systems/DrawBasic3DMeshSystem.h"

using namespace YingLong;

class SingleCubeScene : public Scene
{
public:
	SingleCubeScene();
	virtual ~SingleCubeScene() override {}

	virtual std::string GetName() override { return "SingleCubeScene"; }

protected:
	virtual void OnActive(const std::shared_ptr<Engine>& OwnerEngine, const std::shared_ptr<Scene>& This) override;
	virtual void DrawEntities(float deltatime) override;

private:
	Mesh m_CubeMesh;
	DrawBasic3DMeshSystem m_DrawBasic3DMeshSystem;
};

