#pragma once

#include "scene/Scene.h"

using namespace YingLong;

class SingleCubeScene : public Scene
{
public:
	SingleCubeScene();
	virtual ~SingleCubeScene() override {}

	virtual void Update(float deltatime) override;
};

