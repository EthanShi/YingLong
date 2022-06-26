#include "TestSelector.h"

#include "imgui.h"

#include "testScenes/ClearColor.h"
#include "testScenes/SingleCube.h"
#include "testScenes/CameraMove.h"


TestSelectorScene::TestSelectorScene(std::shared_ptr<Engine>& Engine)
	: Scene()
	, m_Engine(Engine)
	, m_CurrentTest(nullptr)
{
	AddTestScene<ClearColorScene>("Clear color");
	AddTestScene<SingleCubeScene>("Single cube");
	AddTestScene<CameraMoveScene>("Camera move");
}

void TestSelectorScene::DrawImgui(float deltatime)
{
	ImGui::Begin("Tests");

	if (m_CurrentTest.get() == nullptr)
	{
		for (auto& testPair : m_TestScenes)
		{
			if (ImGui::Button(testPair.first.c_str()))
			{
				m_CurrentTest = testPair.second();
				m_Engine->AddScene(m_CurrentTest);
			}
		}
	}
	else
	{
		if (ImGui::Button("<-"))
		{
			m_Engine->RemoveScene(m_CurrentTest);
			m_CurrentTest = nullptr;
		}
	}

	ImGui::End();
}
