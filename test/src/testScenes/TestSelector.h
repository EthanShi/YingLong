#pragma once

#include "scene/Scene.h"
#include "core/Engine.h"

using namespace YingLong;

class TestSelectorScene : public Scene
{
public:
	TestSelectorScene(std::shared_ptr<Engine>& Engine);
	virtual ~TestSelectorScene() override {}

	virtual void DrawImgui(float deltatime) override;

	template<class T>
	void AddTestScene(const std::string& TestName)
	{
		m_TestScenes.push_back(std::make_pair(TestName, []() { return std::make_shared<T>(); }));
	}

private:
	std::shared_ptr<Engine> m_Engine;

	std::vector<std::pair<std::string, std::function<Scene_SPtr()>>> m_TestScenes;
	Scene_SPtr m_CurrentTest;
};