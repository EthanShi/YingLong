#pragma once

#include "SystemBase.h"
#include "renderer/Renderer3D.h"
#include "scene/components/BasicComponents.h"
#include "scene/components/DrawableComponents.h"

namespace YingLong {

	class DrawBasic3DMeshSystem : public SystemBase
	{
	public:
		DrawBasic3DMeshSystem() {};
		virtual ~DrawBasic3DMeshSystem() {};

		virtual void OnOwnerSceneChanged(Scene* OldScene) override {};

		void Draw()
		{
			auto& Reg = GetRegistry();
			auto view = Reg.view<Transform3DComponent, MeshComponent, ShaderComponent>();

			auto PrimaryCamera = GetPrimaryCamera();
			Transform3DComponent& CameraTransform = Reg.get<Transform3DComponent>(PrimaryCamera);
			const Camera3DComponent& Camera = Reg.get<Camera3DComponent>(PrimaryCamera);

			view.each(
				[this, &CameraTransform, &Camera](Transform3DComponent& transform, MeshComponent& mesh, ShaderComponent& shader) {
					shader.m_Uniforms.SetUniform("MVP", Camera.m_Camera.GetPerspective() * CameraTransform.GetViewMatrix() * transform.GetTransform());
					Renderer::Draw(
						mesh.m_MeshRef.GetVertexArray(),
						mesh.m_MeshRef.GetIndexBuffer(),
						shader.m_ShaderID,
						shader.m_Uniforms);
				}
			);
		}
	};
}