#pragma once

#include "scene/Scene.h"

using namespace YingLong;

class ClearColorScene : public Scene
{
public:
	ClearColorScene();
	virtual ~ClearColorScene() override;

	virtual std::string GetName() override { return "ClearColorScene"; }

protected:
	virtual void Update(float deltatime) override;
	virtual void DrawImgui(float deltatime) override;

private:
	float m_ClearColor[4];
};

