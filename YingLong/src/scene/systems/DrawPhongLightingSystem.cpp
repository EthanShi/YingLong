
#include "YingLongPCH.h"

#include "DrawPhongLightingSystem.h"

DEFINE_LOGGER(DrawBasicLightingSystemLog)

void YingLong::DrawBasicLightingSystem::Draw()
{
	auto& Reg = GetRegistry();
	auto LightView = Reg.view<Transform3DComponent, PhongLightComponent>();

	auto FirstLightEntity = LightView.front();
	if (FirstLightEntity == entt::null)
	{
		DrawBasicLightingSystemLog().error("Can not Draw without light source.");
		return;
	}
	if (LightView.size_hint() > 1)
	{
		DrawBasicLightingSystemLog().warn("More than one light source in the Scene, but only first will be used.");
	}
	
	auto PrimaryCamera = GetPrimaryCamera();
	Transform3DComponent& CameraTransform = Reg.get<Transform3DComponent>(PrimaryCamera);
	const Camera3DComponent& Camera = Reg.get<Camera3DComponent>(PrimaryCamera);

	PhongLightComponent& Light = Reg.get<PhongLightComponent>(FirstLightEntity);
	Transform3DComponent& LightTransfrom = Reg.get<Transform3DComponent>(FirstLightEntity);

	auto DrawMesh = [this, &CameraTransform, &Camera, &Light, &LightTransfrom](Transform3DComponent& transform, MeshComponent& mesh, ShaderComponent& shader, PhongMaterialComponent& Material) {
		shader.m_Uniforms.Clear();
		shader.m_Uniforms.SetUniform("Model", transform.GetTransform());
		shader.m_Uniforms.SetUniform("VP", Camera.m_Camera.GetPerspective() * CameraTransform.GetViewMatrix());
		shader.m_Uniforms.SetUniform("InverseModel", transform.GetInverseTransform());
		if (Material.m_Material.GetDiffuseMap().IsValid())
		{
			Material.m_Material.GetDiffuseMap().Bind(0);
			shader.m_Uniforms.SetUniform("material.diffuse", 0);
			Material.m_Material.GetSpecularMap().Bind(1);
			shader.m_Uniforms.SetUniform("material.specular", 1);
		}
		else
		{
			shader.m_Uniforms.SetUniform("material.ambient", Material.m_Material.m_Ambient);
			shader.m_Uniforms.SetUniform("material.diffuse", Material.m_Material.m_Diffuse);
			shader.m_Uniforms.SetUniform("material.specular", Material.m_Material.m_Specular);
		}
		shader.m_Uniforms.SetUniform("material.shininess", Material.m_Material.m_Shininess);
		shader.m_Uniforms.SetUniform("light.ambient", Light.m_Ambient);
		shader.m_Uniforms.SetUniform("light.diffuse", Light.m_Diffuse);
		shader.m_Uniforms.SetUniform("light.specular", Light.m_Specular);
		shader.m_Uniforms.SetUniform("light.positionOrDirection", LightTransfrom.GetPosition());
		shader.m_Uniforms.SetUniform("light.type", (int)Light.Type);
		shader.m_Uniforms.SetUniform("viewPos", CameraTransform.GetPosition());
		Renderer::Draw(
			mesh.m_MeshRef.GetVertexArray(),
			mesh.m_MeshRef.GetIndexBuffer(),
			shader.m_ShaderID,
			shader.m_Uniforms);
	};

	auto MeshView = Reg.view<Transform3DComponent, MeshComponent, ShaderComponent, PhongMaterialComponent>();
	MeshView.each(DrawMesh);
}
