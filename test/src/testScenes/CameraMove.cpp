
#include "YingLongPCH.h"

#include "CameraMove.h"
#include "input/Input.h"
#include "scene/components/BasicComponents.h"
#include "scene/components/DrawableComponents.h"
#include "scene/events/EventTypes.h"


CameraMoveScene::CameraMoveScene()
	: Scene::Scene()
	, m_FreeMovementSystem()
{
	auto& reg = m_Registry;

	// Init cubes mesh & shader
	const auto cubes = reg.create();
	Transform3DComponent& cubesTransform = reg.emplace<Transform3DComponent>(cubes);
	cubesTransform.SetScale(glm::vec3(100.f, 100.f, 100.f));
	cubesTransform.SetForward(glm::vec3(1.f, 1.f, 1.f));

	MeshComponent& MeshComp = reg.emplace<MeshComponent>(cubes);

	MeshComp.mesh.Load("res/models/cube.obj");
	MeshComp.mesh.SetDefaultColor(glm::vec3(1.0f, 0.5f, 0.3f));
	MeshComp.mesh.FillRenderData(false, false, true);

	ShaderComponent& ShaderComp = reg.emplace<ShaderComponent>(cubes);
	ShaderComp.LoadShader("res/shader/basic3D.shader");

	Input& InputInstance = Input::Instance();
	InputInstance.SetCursorMode(CursorMode::CURSOR_DISABLED);

	// Switch cursor mode
	GetInputAction().Bind("SwitchCursorMode", [this]() {
		Input& InputInstance = Input::Instance();
		InputInstance.SetCursorMode(InputInstance.GetCursorMode() == CursorMode::CURSOR_DISABLED ? CursorMode::CURSOR_NORMAL : CursorMode::CURSOR_DISABLED);
		});
}

CameraMoveScene::~CameraMoveScene()
{
	GetInputAction().ClearCallbacks();
}

void CameraMoveScene::OnActive(const std::shared_ptr<Engine>& OwnerEngine, const std::shared_ptr<Scene>& This)
{
	Scene::OnActive(OwnerEngine, This);
	m_FreeMovementSystem.SetOwnerScene(m_WeakThis);
}

void CameraMoveScene::Update(float Deltatime)
{
	m_FreeMovementSystem.Update(Deltatime);
}

void CameraMoveScene::DrawImgui(float Deltatime)
{
}

void CameraMoveScene::CreateDefaultCamera()
{
	Scene::CreateDefaultCamera();
	m_Registry.emplace<FreeMovementComponent>(m_PrimaryCamera);
}
