#include "BasicLighting.h"

#include "scene/components/LightComponent.h"
#include "scene/components/DrawableComponents.h"

BasicLightingScene::BasicLightingScene()
	: CameraMoveScene()
{
}

void BasicLightingScene::OnActive(const std::shared_ptr<Engine>& OwnerEngine, const std::shared_ptr<Scene>& This)
{
	CameraMoveScene::OnActive(OwnerEngine, This);

	CreatePhongLight();

	m_DrawBasicLightingSystem.SetOwnerScene(This);
}

void BasicLightingScene::Update(float Deltatime)
{
	CameraMoveScene::Update(Deltatime);
}

void BasicLightingScene::DrawEntities(float Deltatime)
{
	Scene::DrawEntities(Deltatime);

	m_DrawBasicLightingSystem.Draw();
}

std::string BasicLightingScene::GetShaderPath()
{
	return "res/shader/BasicLighting.shader";
}

void BasicLightingScene::CreatePhongLight()
{
	auto& reg = m_Registry;

	// Init cubes mesh & shader
	const auto LightEntity = reg.create();
	Transform3DComponent& LightTranform = reg.emplace<Transform3DComponent>(LightEntity);
	LightTranform.SetPosition(glm::vec3(0.0f, 0.0f, 1000.0f));
	LightTranform.SetScale(glm::vec3(10.f));

	PhongLightComponent& Light = reg.emplace<PhongLightComponent>(LightEntity, 0.1f, 0.5f, glm::vec3(1.0f, 0.5f, 0.3f));

	reg.emplace<MeshComponent>(LightEntity, m_CubeMesh);
	ShaderComponent& LightShader = reg.emplace<ShaderComponent>(LightEntity);
	LightShader.LoadShader(GetShaderPath());
}
