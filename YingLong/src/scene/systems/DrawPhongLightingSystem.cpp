
#include "YingLongPCH.h"

#include "DrawPhongLightingSystem.h"

DEFINE_LOGGER(DrawBasicLightingSystemLog)

void YingLong::DrawBasicLightingSystem::Draw()
{
	auto& Reg = GetRegistry();

	std::vector<entt::entity> LightEntities;
	uint32 LightCounts = 0;
	std::map<PhongLightComponent*, Transform3DComponent*> Lights;

	auto DirectionalLightView = Reg.view<Transform3DComponent, PhongDirectionalLightComponent>();
	LightCounts += (uint32)DirectionalLightView.size_hint();
	DirectionalLightView.each([&Lights, &LightEntities](auto Entity, Transform3DComponent& Transform, PhongDirectionalLightComponent& Light)
		{
			Lights.emplace(&Light, &Transform);
			LightEntities.push_back(Entity);
		});

	auto PointLightView = Reg.view<Transform3DComponent, PhongPointLightComponent>();
	LightCounts += (uint32)PointLightView.size_hint();
	PointLightView.each([&Lights, &LightEntities](auto Entity, Transform3DComponent& Transform, PhongPointLightComponent& Light)
		{
			Lights.emplace(&Light, &Transform);
			LightEntities.push_back(Entity);
		});

	auto SpotLightView = Reg.view<Transform3DComponent, PhongSpotLightComponent>();
	LightCounts += (uint32)SpotLightView.size_hint();
	SpotLightView.each([&Lights, &LightEntities](auto Entity, Transform3DComponent& Transform, PhongSpotLightComponent& Light)
		{
			Lights.emplace(&Light, &Transform);
			LightEntities.push_back(Entity);
		});

	if (LightCounts == 0)
	{
		DrawBasicLightingSystemLog().error("Can not Draw without light source.");
		return;
	}
	
	auto PrimaryCamera = GetPrimaryCamera();
	Transform3DComponent& CameraTransform = Reg.get<Transform3DComponent>(PrimaryCamera);
	const Camera3DComponent& Camera = Reg.get<Camera3DComponent>(PrimaryCamera);

	auto DrawMesh = [this, &CameraTransform, &Camera, &Lights, &LightEntities](auto Entity, Transform3DComponent& transform, MeshComponent& mesh, ShaderComponent& shader, PhongMaterialComponent& Material) {
		shader.m_Uniforms.Clear();
		shader.m_Uniforms.SetUniform("DrawingLight", std::find(LightEntities.begin(), LightEntities.end(), Entity) == LightEntities.end() ? 0 : 1);
		shader.m_Uniforms.SetUniform("Model", transform.GetTransform());
		shader.m_Uniforms.SetUniform("VP", Camera.m_Camera.GetPerspective() * CameraTransform.GetViewMatrix());
		shader.m_Uniforms.SetUniform("InverseModel", transform.GetInverseTransform());
		if (Material.m_Material.GetDiffuseMap().IsValid())
		{
			shader.m_Uniforms.SetUniform("useMaterialWithMap", 1);
			Material.m_Material.GetDiffuseMap().Bind(0);
			shader.m_Uniforms.SetUniform("materialWithMap.diffuse", 0);
			Material.m_Material.GetSpecularMap().Bind(1);
			shader.m_Uniforms.SetUniform("materialWithMap.specular", 1);
		}
		else
		{
			shader.m_Uniforms.SetUniform("useMaterialWithMap", 0);
			shader.m_Uniforms.SetUniform("material.ambient", Material.m_Material.m_Ambient);
			shader.m_Uniforms.SetUniform("material.diffuse", Material.m_Material.m_Diffuse);
			shader.m_Uniforms.SetUniform("material.specular", Material.m_Material.m_Specular);
		}
		shader.m_Uniforms.SetUniform("material.shininess", Material.m_Material.m_Shininess);
		
		shader.m_Uniforms.SetUniform("LightCounts", (int)Lights.size());
		int8 Index = 0;
		for (auto& Light : Lights)
		{
			PhongLightComponent* LightComp = Light.first;
			Transform3DComponent* Transform = Light.second;
			std::string Perfix = "Lights[" + std::to_string(Index) + "].";
			shader.m_Uniforms.SetUniform(Perfix + "ambient", LightComp->m_Ambient);
			shader.m_Uniforms.SetUniform(Perfix + "diffuse", LightComp->m_Diffuse);
			shader.m_Uniforms.SetUniform(Perfix + "specular", LightComp->m_Specular);
			if (LightComp->Type == PhongLightingType::Directional)
			{
				shader.m_Uniforms.SetUniform(Perfix + "positionOrDirection", Transform->GetForward());
			}
			else
			{
				shader.m_Uniforms.SetUniform(Perfix + "positionOrDirection", Transform->GetPosition());
			}
			shader.m_Uniforms.SetUniform(Perfix + "type", (int)LightComp->Type);
			if (LightComp->Type == PhongLightingType::Point || LightComp->Type == PhongLightingType::Spot)
			{
				PhongPointLightComponent* PointLight = static_cast<PhongPointLightComponent*>(LightComp);
				shader.m_Uniforms.SetUniform(Perfix + "constant", PointLight->m_Constant);
				shader.m_Uniforms.SetUniform(Perfix + "linear", PointLight->m_Linear);
				shader.m_Uniforms.SetUniform(Perfix + "quadratic", PointLight->m_Quadratic);
				if (LightComp->Type == PhongLightingType::Spot)
				{
					PhongSpotLightComponent* SpotLight = static_cast<PhongSpotLightComponent*>(LightComp);
					shader.m_Uniforms.SetUniform(Perfix + "spotDirection", Transform->GetForward());
					shader.m_Uniforms.SetUniform(Perfix + "innerCutOffInCos", SpotLight->m_InnerCutOffInCos);
					shader.m_Uniforms.SetUniform(Perfix + "outerCutOffInCos", SpotLight->m_OuterCutOffInCos);
				}
			}
			Index++;
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
