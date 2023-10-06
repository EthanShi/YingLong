#include "clearColor.h"

#include "imgui.h"
#include "glm/glm.hpp"

#include "renderer/Renderer3D.h"

ClearColorScene::ClearColorScene()
	: Scene()
{
	BackgroundColorCache = Renderer::GetBackgroundColor();
}

ClearColorScene::~ClearColorScene()
{
	Renderer::SetBackgroundColor(BackgroundColorCache);
}

void ClearColorScene::Update(float deltatime)
{
	Renderer::SetBackgroundColor(glm::vec4(
		ClearColor[0],
		ClearColor[1],
		ClearColor[2],
		ClearColor[3]));
}

void ClearColorScene::DrawImgui(float deltatime)
{
	ImGui::Begin("clear color");
	ImGui::ColorEdit4("Clear Color", ClearColor);
	ImGui::End();
}
