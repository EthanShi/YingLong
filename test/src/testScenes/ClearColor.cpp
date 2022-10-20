#include "clearColor.h"

#include "imgui.h"
#include "glm/glm.hpp"

#include "renderer/Renderer3D.h"

ClearColorScene::ClearColorScene()
	: Scene()
{
	m_BackgroundColorCache = Renderer::GetBackgroundColor();
}

ClearColorScene::~ClearColorScene()
{
	Renderer::SetBackgroundColor(m_BackgroundColorCache);
}

void ClearColorScene::Update(float deltatime)
{
	Renderer::SetBackgroundColor(glm::vec4(
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
