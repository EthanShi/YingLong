#include "TestSelector.h"

#include "imgui.h"

#include "testScenes/ClearColor.h"
#include "testScenes/SingleCube.h"
#include "testScenes/CameraMove.h"
#include "testScenes/PhongLighting.h"


TestSelectorScene::TestSelectorScene()
	: Scene()
	, m_CurrentTest(nullptr)
{
	AddTestScene<ClearColorScene>("Clear color");
	AddTestScene<SingleCubeScene>("Single cube");
	AddTestScene<CameraMoveScene>("Camera move");
	AddTestScene<PhongLightingScene>("Phong lighting");
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
				if (auto& Engine = m_Engine.lock())
				{
					Engine->AddScene(m_CurrentTest);
				}
			}
		}
	}
	else
	{
		if (ImGui::Button("<-"))
		{
			if (auto& Engine = m_Engine.lock())
			{
				Engine->RemoveScene(m_CurrentTest);
			}
			m_CurrentTest = nullptr;
		}
	}

	ImGui::End();
}
