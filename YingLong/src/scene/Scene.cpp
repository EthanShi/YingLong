
#include "YingLongPCH.h"

#include "Scene.h"
#include "renderer/Renderer3D.h"

#include "components/BasicComponents.h"
#include "components/DrawableComponents.h"

namespace YingLong {

	Scene::Scene()
		: m_BackgroundColor(0.3f, 0.3f, 0.3f, 1.0f)
	{
		CreateDefaultCamera();
	}

	Scene::~Scene()
	{
	}

	void Scene::Tick(float deltatime)
	{
		Renderer::SetClearColor(m_BackgroundColor);
		InnerUpdate(deltatime);
		InnerDrawEntities(deltatime);
		InnerDrawImgui(deltatime);
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
		auto& reg = GetRegistry();
		const auto camera = reg.create();
		Transform3DComponent& cameraTransform = reg.emplace<Transform3DComponent>(camera);
		cameraTransform.SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
		reg.emplace<Camera3DComponent>(camera);
		SetPrimaryCamera(camera);
	}

	void Scene::InnerUpdate(float deltatime)
	{
		Update(deltatime);
	}

	void Scene::InnerDrawEntities(float deltatime)
	{
		DrawEntities(deltatime);
	}

	void Scene::InnerDrawImgui(float deltatime)
	{
		DrawImgui(deltatime);
	}
}