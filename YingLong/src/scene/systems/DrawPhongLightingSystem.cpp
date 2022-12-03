
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
		shader.uniforms.SetUniform("Model", transform.GetTransform());
		shader.uniforms.SetUniform("VP", Camera.Camera.GetPerspective() * CameraTransform.GetViewMatrix());
		shader.uniforms.SetUniform("InverseModel", transform.GetInverseTransform());
		shader.uniforms.SetUniform("material.ambient", Material.Material.Ambient);
		shader.uniforms.SetUniform("material.diffuse", Material.Material.Diffuse);
		shader.uniforms.SetUniform("material.specular", Material.Material.Specular);
		shader.uniforms.SetUniform("material.shininess", Material.Material.Shininess);
		shader.uniforms.SetUniform("lightColor", Light.m_LightColor),
			shader.uniforms.SetUniform("lightPos", LightTransfrom.GetPosition());
		shader.uniforms.SetUniform("viewPos", CameraTransform.GetPosition());
		Renderer::Draw(
			mesh.MeshRef.GetVertexArray(),
			mesh.MeshRef.GetIndexBuffer(),
			shader.shaderID,
			shader.uniforms);
	};

	auto MeshView = Reg.view<Transform3DComponent, MeshComponent, ShaderComponent, PhongMaterialComponent>();
	MeshView.each(DrawMesh);
}
