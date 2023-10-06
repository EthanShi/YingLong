#pragma once

#include "scene/Scene.h"
#include "core/Engine.h"

using namespace YingLong;

class TestSelectorScene : public Scene
{
public:
	TestSelectorScene();
	virtual ~TestSelectorScene() override {}

	virtual void DrawImgui(float deltatime) override;

	virtual std::string GetName() override { return "TestSelectorScene"; }

	template<class T>
	void AddTestScene(const std::string& TestName)
	{
		TestScenes.push_back(std::make_pair(TestName, []() { return std::make_shared<T>(); }));
	}

private:
	std::vector<std::pair<std::string, std::function<std::shared_ptr<Scene>()>>> TestScenes;
	std::shared_ptr<Scene> CurrentTest;
};