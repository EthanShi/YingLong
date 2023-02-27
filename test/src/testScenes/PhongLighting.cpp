#include "PhongLighting.h"

#include "scene/components/PhongLightingComponent.h"
#include "scene/components/DrawableComponents.h"

PhongLightingScene::PhongLightingScene()
	: CameraMoveScene()
{
	m_TestMaterials["emerald"] = PhongMaterial(glm::vec3(0.0215, 0.1745, 0.0215), glm::vec3(0.07568, 0.61424, 0.07568), glm::vec3(0.633, 0.727811,0.633), 0.6f * 128);
	m_TestMaterials["jade"] = PhongMaterial(glm::vec3(0.135, 0.2225, 0.1575), glm::vec3(0.54, 0.89, 0.63), glm::vec3(0.316228, 0.316228, 0.316228), 0.1f * 128);
	m_TestMaterials["obsidian"] = PhongMaterial(glm::vec3(0.05375, 0.05, 0.06625), glm::vec3(0.18275, 0.17, 0.22525), glm::vec3(0.332741, 0.328634, 0.346435), 0.3f * 128);
	m_TestMaterials["pearl"] = PhongMaterial(glm::vec3(0.25, 0.20725, 0.20725), glm::vec3(1, 0.829, 0.829), glm::vec3(0.296648, 0.296648, 0.296648), 0.088f * 128);
	m_TestMaterials["ruby"] = PhongMaterial(glm::vec3(0.1745, 0.01175, 0.01175), glm::vec3(0.61424, 0.04136, 0.04136), glm::vec3(0.727811, 0.626959, 0.626959), 0.6f * 128);
	m_TestMaterials["turquoise"] = PhongMaterial(glm::vec3(0.1, 0.18725, 0.1745), glm::vec3(0.396, 0.74151, 0.69102), glm::vec3(0.297254, 0.30829, 0.306678), 0.1f * 128);
	m_TestMaterials["brass"] = PhongMaterial(glm::vec3(0.329412, 0.223529, 0.027451), glm::vec3(0.780392, 0.568627, 0.113725), glm::vec3(0.992157, 0.941176, 0.807843), 0.21794872f * 128);
	m_TestMaterials["bronze"] = PhongMaterial(glm::vec3(0.2125, 0.1275, 0.054), glm::vec3(0.714, 0.4284, 0.18144), glm::vec3(0.393548, 0.271906, 0.166721), 0.2f * 128);
	m_TestMaterials["chrome"] = PhongMaterial(glm::vec3(0.25, 0.25, 0.25), glm::vec3(0.4, 0.4, 0.4), glm::vec3(0.774597, 0.774597, 0.774597), 0.6f * 128);
	m_TestMaterials["copper"] = PhongMaterial(glm::vec3(0.19125, 0.0735, 0.0225), glm::vec3(0.7038, 0.27048, 0.0828), glm::vec3(0.256777, 0.137622, 0.086014), 0.1f * 128);
	m_TestMaterials["gold"] = PhongMaterial(glm::vec3(0.24725, 0.1995, 0.0745), glm::vec3(0.75164, 0.60648, 0.22648), glm::vec3(0.628281, 0.555802, 0.366065), 0.4f * 128);
	m_TestMaterials["silver"] = PhongMaterial(glm::vec3(0.19225, 0.19225, 0.19225), glm::vec3(0.50754, 0.50754, 0.50754), glm::vec3(0.508273, 0.508273, 0.508273), 0.4f * 128);
	m_TestMaterials["black plastic"] = PhongMaterial(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.01, 0.01, 0.01), glm::vec3(0.50, 0.50, 0.50), 0.25f * 128);
	m_TestMaterials["cyan plastic"] = PhongMaterial(glm::vec3(0.0, 0.1, 0.06), glm::vec3(0.0, 0.50980392, 0.50980392), glm::vec3(0.50196078, 0.50196078, 0.50196078), 0.25f * 128);
	m_TestMaterials["green plastic"] = PhongMaterial(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.1, 0.35, 0.1), glm::vec3(0.45, 0.55, 0.45), 0.25f * 128);
	m_TestMaterials["red plastic"] = PhongMaterial(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.5, 0.0, 0.0), glm::vec3(0.7, 0.6, 0.6), 0.25f * 128);
	m_TestMaterials["white plastic"] = PhongMaterial(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.55, 0.55, 0.55), glm::vec3(0.70, 0.70, 0.70), 0.25f * 128);
	m_TestMaterials["yellow plastic"] = PhongMaterial(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.5, 0.5, 0.0), glm::vec3(0.60, 0.60, 0.50), 0.25f * 128);
	m_TestMaterials["black rubber"] = PhongMaterial(glm::vec3(0.02, 0.02, 0.02), glm::vec3(0.01, 0.01, 0.01), glm::vec3(0.4, 0.4, 0.4), 0.078125f * 128);
	m_TestMaterials["cyan rubber"] = PhongMaterial(glm::vec3(0.0, 0.05, 0.05), glm::vec3(0.4, 0.5, 0.5), glm::vec3(0.04, 0.7, 0.7), 0.078125f * 128);
	m_TestMaterials["green rubber"] = PhongMaterial(glm::vec3(0.0, 0.05, 0.0), glm::vec3(0.4, 0.5, 0.4), glm::vec3(0.04, 0.7, 0.04), 0.078125f * 128);
	m_TestMaterials["red rubber"] = PhongMaterial(glm::vec3(0.05, 0.0, 0.0), glm::vec3(0.5, 0.4, 0.4), glm::vec3(0.7, 0.04, 0.04), 0.078125f * 128);
	m_TestMaterials["white rubber"] = PhongMaterial(glm::vec3(0.05, 0.05, 0.05), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.7, 0.7, 0.7), 0.078125f * 128);
	m_TestMaterials["yellow rubber"] = PhongMaterial(glm::vec3(0.05, 0.05, 0.0), glm::vec3(0.5, 0.5, 0.4), glm::vec3(0.7, 0.7, 0.04), 0.078125f * 128);
	m_TestMaterials["Container"] = PhongMaterial("res/texture/container2.png", "res/texture/container2_specular.png", glm::vec3(0.7, 0.7, 0.04), 0.078125f * 128);
}

void PhongLightingScene::OnActive(const std::shared_ptr<Engine>& OwnerEngine, const std::shared_ptr<Scene>& This)
{
	CameraMoveScene::OnActive(OwnerEngine, This);

	CreatePhongLight();

	m_DrawBasicLightingSystem.SetOwnerScene(This);
}

void PhongLightingScene::Update(float Deltatime)
{
	CameraMoveScene::Update(Deltatime);
}

void PhongLightingScene::DrawImgui(float Deltatime)
{
	ImGui::Begin("Select Material");

	std::vector<const char*> MaterialNames;
	for (auto& MaterialPair : m_TestMaterials)
	{
		MaterialNames.push_back(MaterialPair.first.c_str());
	}

	ImGui::Combo("Material", &m_CurrentMaterialIndex, MaterialNames.data(), int(MaterialNames.size()));
	m_CurrentMaterialName = MaterialNames[m_CurrentMaterialIndex];

	ImGui::End();
}

void PhongLightingScene::DrawEntities(float Deltatime)
{
	Scene::DrawEntities(Deltatime);

	auto CurMat = m_TestMaterials.find(m_CurrentMaterialName);
	if (CurMat != m_TestMaterials.end())
	{
		auto ReplaceMaterialView = m_Registry.view<PhongMaterialComponent>(entt::exclude<PhongLightComponent>);
		ReplaceMaterialView.each([&CurMat](PhongMaterialComponent& Material) {
			Material.m_Material = CurMat->second;
		});
	}

	m_DrawBasicLightingSystem.Draw();
}

void PhongLightingScene::CreateACube(const glm::vec3& Position, const glm::vec3& Forward, const glm::vec3& Scale)
{
	auto& reg = m_Registry;

	// Init cubes mesh & shader
	const auto cube = reg.create();
	Transform3DComponent& cubesTranform = reg.emplace<Transform3DComponent>(cube);
	cubesTranform.SetPosition(Position);
	cubesTranform.SetScale(Scale);
	cubesTranform.SetForward(Forward);

	MeshComponent& MeshComp = reg.emplace<MeshComponent>(cube, m_CubeMesh);

	ShaderComponent& ShaderComp = reg.emplace<ShaderComponent>(cube);
	ShaderComp.LoadShader(GetShaderPath());

	reg.emplace<PhongMaterialComponent>(cube, m_TestMaterials["emerald"]);
}

std::string PhongLightingScene::GetShaderPath()
{
	return "../YingLong/res/shaders/PhongLighting.shader";
}

void PhongLightingScene::CreatePhongLight()
{
	auto& reg = m_Registry;

	// Init cubes mesh & shader
	const auto LightEntity = reg.create();
	Transform3DComponent& LightTranform = reg.emplace<Transform3DComponent>(LightEntity);
	LightTranform.SetPosition(glm::vec3(0.0f, 0.0f, 500.0f));
	LightTranform.SetScale(glm::vec3(10.f));

	PhongSpotLightComponent& PointLight = reg.emplace<PhongSpotLightComponent>(
		LightEntity,
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		1.f, 0.045f, 0.0075f,
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::cos(glm::radians(12.5f)),
		glm::cos(glm::radians(17.5f)));

	reg.emplace<MeshComponent>(LightEntity, m_CubeMesh);
	ShaderComponent& LightShader = reg.emplace<ShaderComponent>(LightEntity);
	LightShader.LoadShader(GetShaderPath());

	reg.emplace<PhongMaterialComponent>(
		LightEntity,
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f), 32);
}
