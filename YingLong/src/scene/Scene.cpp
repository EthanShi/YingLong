
#include "YingLongPCH.h"

#include "core/Engine.h"
#include "Scene.h"
#include "renderer/Renderer3D.h"

#include "components/BasicComponents.h"
#include "components/DrawableComponents.h"

namespace YingLong {

	Scene::Scene()
		: m_Registry()
		, m_Dispatcher()
		, m_BackgroundColor(0.3f, 0.3f, 0.3f, 1.0f)
		, m_PrimaryCamera()
	{}

	Scene::~Scene()
	{
	}

	void Scene::OnActive(const std::shared_ptr<Engine>& OwnerEngine, const std::shared_ptr<Scene>& This)
	{
		m_Engine = OwnerEngine;
		m_WeakThis = This;
		CreateDefaultCamera();
	}

	void Scene::Tick(float deltatime)
	{
		m_Dispatcher.update();
		Renderer::SetClearColor(m_BackgroundColor);
		Update(deltatime);
		DrawEntities(deltatime);
		DrawImgui(deltatime);
	}

	void Scene::Update(float deltatime)
	{
	}

	void Scene::DrawEntities(float deltatime)
	{
		auto view = m_Registry.view<Transform3DComponent, MeshComponent, ShaderComponent>();

		Transform3DComponent& CameraTransform = m_Registry.get<Transform3DComponent>(m_PrimaryCamera);
		const Camera3DComponent& Camera = m_Registry.get<Camera3DComponent>(m_PrimaryCamera);

		view.each(
			[this, &CameraTransform, &Camera](Transform3DComponent& transform, MeshComponent& mesh, ShaderComponent& shader) {
				Renderer::Draw(
					mesh.mesh.GetVertexArray(),
					mesh.mesh.GetIndexBuffer(),
					shader.shaderID,
					transform.GetTransform(),
					Camera.Camera.GetPerspective(),
					CameraTransform.GetTransform()
				);
			}
		);
	}

	void Scene::DrawImgui(float deltatime)
	{
	}

	void Scene::CreateDefaultCamera()
	{
		m_PrimaryCamera = m_Registry.create();
		Transform3DComponent& cameraTransform = m_Registry.emplace<Transform3DComponent>(m_PrimaryCamera);
		cameraTransform.SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
		m_Registry.emplace<Camera3DComponent>(m_PrimaryCamera);
	}
}