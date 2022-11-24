
#include "YingLongPCH.h"

#include "DrawBasicLightingSystem.h"

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
	MeshComponent& LightMesh = Reg.get<MeshComponent>(FirstLightEntity);
	ShaderComponent& LightShader = Reg.get<ShaderComponent>(FirstLightEntity);
	LightShader.uniforms.SetUniform("Model", LightTransfrom.GetTransform());
	LightShader.uniforms.SetUniform("VP", Camera.Camera.GetPerspective() * CameraTransform.GetViewMatrix());
	LightShader.uniforms.SetUniform("InverseModel", LightTransfrom.GetInverseTransform());
	LightShader.uniforms.SetUniform("ambientStrength", 1.0f);
	LightShader.uniforms.SetUniform("specularStrength", 0.0f);
	LightShader.uniforms.SetUniform("lightColor", Light.m_LightColor),
	LightShader.uniforms.SetUniform("lightPos", LightTransfrom.GetPosition());
	LightShader.uniforms.SetUniform("viewPos", CameraTransform.GetPosition());
	Renderer::Draw(
		LightMesh.MeshRef.GetVertexArray(),
		LightMesh.MeshRef.GetIndexBuffer(),
		LightShader.shaderID,
		LightShader.uniforms);

	auto MeshView = Reg.view<Transform3DComponent, MeshComponent, ShaderComponent>();
	MeshView.each(
		[this, &CameraTransform, &Camera, &Light, &LightTransfrom](Transform3DComponent& transform, MeshComponent& mesh, ShaderComponent& shader) {
			shader.uniforms.SetUniform("Model", transform.GetTransform());
			shader.uniforms.SetUniform("VP", Camera.Camera.GetPerspective() * CameraTransform.GetViewMatrix());
			shader.uniforms.SetUniform("InverseModel", transform.GetInverseTransform());
			shader.uniforms.SetUniform("ambientStrength", Light.m_AmbientStrength);
			shader.uniforms.SetUniform("specularStrength", Light.m_SpecularStrength);
			shader.uniforms.SetUniform("lightColor", Light.m_LightColor),
			shader.uniforms.SetUniform("lightPos", LightTransfrom.GetPosition());
			shader.uniforms.SetUniform("viewPos", CameraTransform.GetPosition());
			Renderer::Draw(
				mesh.MeshRef.GetVertexArray(),
				mesh.MeshRef.GetIndexBuffer(),
				shader.shaderID,
				shader.uniforms);
		}
	);
}
