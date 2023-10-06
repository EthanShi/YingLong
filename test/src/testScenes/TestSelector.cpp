#include "TestSelector.h"

#include "imgui.h"

#include "testScenes/ClearColor.h"
#include "testScenes/SingleCube.h"
#include "testScenes/CameraMove.h"
#include "testScenes/PhongLighting.h"
#include "testScenes/DepthTest.h"


TestSelectorScene::TestSelectorScene()
	: Scene()
	, CurrentTest(nullptr)
{
	AddTestScene<ClearColorScene>("Clear color");
	AddTestScene<SingleCubeScene>("Single cube");
	AddTestScene<CameraMoveScene>("Camera move");
	AddTestScene<PhongLightingScene>("Phong lighting");
	AddTestScene<DepthTestScene>("Depth test");
}

void TestSelectorScene::DrawImgui(float deltatime)
{
	ImGui::Begin("Tests");

	if (CurrentTest.get() == nullptr)
	{
		for (auto& testPair : TestScenes)
		{
			if (ImGui::Button(testPair.first.c_str()))
			{
				CurrentTest = testPair.second();
				if (const auto SharedEngine = OwnerEngine.lock())
				{
					SharedEngine->AddScene(CurrentTest);
				}
			}
		}
	}
	else
	{
		if (ImGui::Button("<-"))
		{
			if (const auto SharedEngine = OwnerEngine.lock())
			{
				SharedEngine->RemoveScene(CurrentTest);
			}
			CurrentTest = nullptr;
		}
	}

	ImGui::End();
}
