
#include "YingLongPCH.h"

#include "Scene.h"
#include "renderer/Renderer3D.h"

namespace YingLong {

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::Tick(float deltatime)
	{
		InnerUpdate(deltatime);
		InnerDrawEntities(deltatime);
		InnerDrawImgui(deltatime);
	}

	void Scene::Update(float deltatime)
	{
	}

	void Scene::DrawEntities(float deltatime)
	{
	}

	void Scene::DrawImgui(float deltatime)
	{
	}

	void Scene::InnerUpdate(float deltatime)
	{
		Update(deltatime);
	}

	void Scene::InnerDrawEntities(float deltatime)
	{
		Renderer::SetClearColor(m_BackgroundColor);
		Renderer::Clear();

		DrawEntities(deltatime);
	}

	void Scene::InnerDrawImgui(float deltatime)
	{
		DrawImgui(deltatime);
	}
}