
#include "YingLongPCH.h"

#include "Scene.h"
#include "renderer/Renderer3D.h"

#include "components/BasicComponents.h"
#include "components/DrawableComponents.h"

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

		Renderer::SetClearColor(m_BackgroundColor);
		Renderer::Clear();

		InnerDrawEntities(deltatime);
		InnerDrawImgui(deltatime);
	}

	void Scene::Update(float deltatime)
	{
	}

	void Scene::DrawEntities(float deltatime)
	{
		auto view = m_Registry.view<const Transform3DComponent, MeshComponent, ShaderComponent>();

		const Transform3DComponent& CameraTransform = m_Registry.get<Transform3DComponent>(PrimaryCamera);
		const Camera3DComponent& Camera = m_Registry.get<Camera3DComponent>(PrimaryCamera);

		view.each(
			[this, &CameraTransform, &Camera](const Transform3DComponent& transform, MeshComponent& mesh, ShaderComponent& shader) {
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