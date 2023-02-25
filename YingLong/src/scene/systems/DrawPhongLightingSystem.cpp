
#include "YingLongPCH.h"

#include "DrawPhongLightingSystem.h"

DEFINE_LOGGER(DrawBasicLightingSystemLog)

void YingLong::DrawBasicLightingSystem::Draw()
{
	auto& Reg = GetRegistry();

	uint32 LightCounts = 0;
	PhongLightComponent* Light;
	Transform3DComponent* LightTransform;

	[&Reg, &LightCounts, &Light, &LightTransform]() {
		auto DirectionalLightView = Reg.view<Transform3DComponent, PhongDirectionalLightComponent>();
		auto FirstLightEntity = DirectionalLightView.front();
		if (FirstLightEntity != entt::null)
		{
			LightCounts = DirectionalLightView.size_hint();
			Light = &Reg.get<PhongDirectionalLightComponent>(FirstLightEntity);
			LightTransform = &Reg.get<Transform3DComponent>(FirstLightEntity);
			return;
		}

		auto PointLightView = Reg.view<Transform3DComponent, PhongPointLightComponent>();
		FirstLightEntity = PointLightView.front();
		if (FirstLightEntity != entt::null)
		{
			LightCounts = PointLightView.size_hint();
			Light = &Reg.get<PhongPointLightComponent>(FirstLightEntity);
			LightTransform = &Reg.get<Transform3DComponent>(FirstLightEntity);
			return;
		}

		auto SpotLightView = Reg.view<Transform3DComponent, PhongSpotLightComponent>();
		FirstLightEntity = SpotLightView.front();
		if (FirstLightEntity != entt::null)
		{
			LightCounts = SpotLightView.size_hint();
			Light = &Reg.get<PhongSpotLightComponent>(FirstLightEntity);
			LightTransform = &Reg.get<Transform3DComponent>(FirstLightEntity);
		}
	}();

	if (LightCounts == 0)
	{
		DrawBasicLightingSystemLog().error("Can not Draw without light source.");
		return;
	}
	if (LightCounts > 1)
	{
		DrawBasicLightingSystemLog().warn("More than one light source in the Scene, but only first will be used.");
	}
	
	auto PrimaryCamera = GetPrimaryCamera();
	Transform3DComponent& CameraTransform = Reg.get<Transform3DComponent>(PrimaryCamera);
	const Camera3DComponent& Camera = Reg.get<Camera3DComponent>(PrimaryCamera);

	auto DrawMesh = [this, &CameraTransform, &Camera, &Light, &LightTransform](Transform3DComponent& transform, MeshComponent& mesh, ShaderComponent& shader, PhongMaterialComponent& Material) {
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
		shader.m_Uniforms.SetUniform("light.ambient", Light->m_Ambient);
		shader.m_Uniforms.SetUniform("light.diffuse", Light->m_Diffuse);
		shader.m_Uniforms.SetUniform("light.specular", Light->m_Specular);
		shader.m_Uniforms.SetUniform("light.positionOrDirection", LightTransform->GetPosition());
		shader.m_Uniforms.SetUniform("light.type", (int)Light->Type);
		if (Light->Type == PhongLightingType::Point || Light->Type == PhongLightingType::Spot)
		{
			PhongPointLightComponent* PointLight = static_cast<PhongPointLightComponent*>(Light);
			shader.m_Uniforms.SetUniform("light.constant", PointLight->m_Constant);
			shader.m_Uniforms.SetUniform("light.linear", PointLight->m_Linear);
			shader.m_Uniforms.SetUniform("light.quadratic", PointLight->m_Quadratic);
			if (Light->Type == PhongLightingType::Spot)
			{
				PhongSpotLightComponent* SpotLight = static_cast<PhongSpotLightComponent*>(Light);
				shader.m_Uniforms.SetUniform("light.spotDirection", SpotLight->m_SpotDirection);
				shader.m_Uniforms.SetUniform("light.innerCutOffInCos", SpotLight->m_InnerCutOffInCos);
				shader.m_Uniforms.SetUniform("light.outerCutOffInCos", SpotLight->m_OuterCutOffInCos);
			}
		}

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
