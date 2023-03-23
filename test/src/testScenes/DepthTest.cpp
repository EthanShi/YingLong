#include "DepthTest.h"
#include "renderer/Mesh.h"
#include "input/Input.h"

#include "scene/components/BasicComponents.h"
#include "scene/components/DrawableComponents.h"

DepthTestScene::DepthTestScene()
	: Scene::Scene()
	, m_FreeMovementSystem()
	, m_DrawBasic3DMeshSystem()
{
	Input& InputInstance = Input::Instance();
	InputInstance.SetCursorMode(CursorMode::CURSOR_DISABLED);

	auto& reg = m_Registry;

	// Init cubes mesh & shader
	const auto cubes = reg.create();
	Transform3DComponent& cubesTransform = reg.emplace<Transform3DComponent>(cubes);
	cubesTransform.SetScale(glm::vec3(100.f, 100.f, 100.f));
	cubesTransform.SetForward(glm::vec3(1.f, 1.f, 1.f));

	m_CubeMesh.LoadObjData("res/models/cube.obj");
	m_CubeMesh.SetDefaultColor(glm::vec3(1.0f, 0.5f, 0.3f));
	m_CubeMesh.FillRenderData(true, false, true);

	MeshComponent& MeshComp = reg.emplace<MeshComponent>(cubes, m_CubeMesh);

	ShaderComponent& ShaderComp = reg.emplace<ShaderComponent>(cubes);
	ShaderComp.LoadShader("../YingLong/res/shaders/basic3D.shader");


}

void DepthTestScene::OnActive(const std::shared_ptr<Engine>& OwnerEngine, const std::shared_ptr<Scene>& This)
{
	Scene::OnActive(OwnerEngine, This);

	m_FreeMovementSystem.SetOwnerScene(This);
	m_DrawBasic3DMeshSystem.SetOwnerScene(This);
}

void DepthTestScene::Update(float deltatime)
{
	m_FreeMovementSystem.Update(deltatime);
}

void DepthTestScene::DrawEntities(float deltatime)
{
	Scene::DrawEntities(deltatime);

	m_DrawBasic3DMeshSystem.Draw();
}

void DepthTestScene::CreateDefaultCamera()
{
	Scene::CreateDefaultCamera();
	m_Registry.emplace<FreeMovementComponent>(m_PrimaryCamera);
}