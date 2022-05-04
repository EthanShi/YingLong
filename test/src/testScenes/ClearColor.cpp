#include "clearColor.h"

#include "imgui.h"
#include "glm/glm.hpp"

ClearColorScene::ClearColorScene()
	: Scene()
	, m_ClearColor{ 0.5f, 0.5f, 0.5f, 1.0f }
{
}

ClearColorScene::~ClearColorScene()
{
}

void ClearColorScene::Update(float deltatime)
{
	SetBackgroundColor(glm::vec4(
		m_ClearColor[0],
		m_ClearColor[1],
		m_ClearColor[2],
		m_ClearColor[3]));
}

void ClearColorScene::DrawImgui(float deltatime)
{
	ImGui::Begin("clear color");
	ImGui::ColorEdit4("Clear Color", m_ClearColor);
	ImGui::End();
}
