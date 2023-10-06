
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
		shader.Uniforms.Clear();
		shader.Uniforms.SetUniform("DrawingLight", std::find(LightEntities.begin(), LightEntities.end(), Entity) == LightEntities.end() ? 0 : 1);
		shader.Uniforms.SetUniform("Model", transform.GetTransform());
		shader.Uniforms.SetUniform("VP", Camera.Camera.GetPerspective() * CameraTransform.GetViewMatrix());
		shader.Uniforms.SetUniform("InverseModel", transform.GetInverseTransform());
		if (Material.Material.GetDiffuseMap().IsValid())
		{
			shader.Uniforms.SetUniform("useMaterialWithMap", 1);
			Material.Material.GetDiffuseMap().Bind(0);
			shader.Uniforms.SetUniform("materialWithMap.diffuse", 0);
			Material.Material.GetSpecularMap().Bind(1);
			shader.Uniforms.SetUniform("materialWithMap.specular", 1);
		}
		else
		{
			shader.Uniforms.SetUniform("useMaterialWithMap", 0);
			shader.Uniforms.SetUniform("material.ambient", Material.Material.Ambient);
			shader.Uniforms.SetUniform("material.diffuse", Material.Material.Diffuse);
			shader.Uniforms.SetUniform("material.specular", Material.Material.Specular);
		}
		shader.Uniforms.SetUniform("material.shininess", Material.Material.Shininess);
		
		shader.Uniforms.SetUniform("LightCounts", (int)Lights.size());
		int8 Index = 0;
		for (auto& Light : Lights)
		{
			PhongLightComponent* LightComp = Light.first;
			Transform3DComponent* Transform = Light.second;
			std::string Perfix = "Lights[" + std::to_string(Index) + "].";
			shader.Uniforms.SetUniform(Perfix + "ambient", LightComp->Ambient);
			shader.Uniforms.SetUniform(Perfix + "diffuse", LightComp->Diffuse);
			shader.Uniforms.SetUniform(Perfix + "specular", LightComp->Specular);
			if (LightComp->Type == PhongLightingType::Directional)
			{
				shader.Uniforms.SetUniform(Perfix + "positionOrDirection", Transform->GetForward());
			}
			else
			{
				shader.Uniforms.SetUniform(Perfix + "positionOrDirection", Transform->GetPosition());
			}
			shader.Uniforms.SetUniform(Perfix + "type", (int)LightComp->Type);
			if (LightComp->Type == PhongLightingType::Point || LightComp->Type == PhongLightingType::Spot)
			{
				PhongPointLightComponent* PointLight = static_cast<PhongPointLightComponent*>(LightComp);
				shader.Uniforms.SetUniform(Perfix + "constant", PointLight->Constant);
				shader.Uniforms.SetUniform(Perfix + "linear", PointLight->Linear);
				shader.Uniforms.SetUniform(Perfix + "quadratic", PointLight->Quadratic);
				if (LightComp->Type == PhongLightingType::Spot)
				{
					PhongSpotLightComponent* SpotLight = static_cast<PhongSpotLightComponent*>(LightComp);
					shader.Uniforms.SetUniform(Perfix + "spotDirection", Transform->GetForward());
					shader.Uniforms.SetUniform(Perfix + "innerCutOffInCos", SpotLight->InnerCutOffInCos);
					shader.Uniforms.SetUniform(Perfix + "outerCutOffInCos", SpotLight->OuterCutOffInCos);
				}
			}
			Index++;
		}

		shader.Uniforms.SetUniform("viewPos", CameraTransform.GetPosition()); 
		Renderer::Draw(
			mesh.MeshRef.GetVertexArray(),
			mesh.MeshRef.GetIndexBuffer(),
			shader.ShaderID,
			shader.Uniforms);
	};

	auto MeshView = Reg.view<Transform3DComponent, MeshComponent, ShaderComponent, PhongMaterialComponent>();
	MeshView.each(DrawMesh);
}
