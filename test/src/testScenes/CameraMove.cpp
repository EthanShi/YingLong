#include "CameraMove.h"

#include "YingLongPCH.h"

#include "glad/gl.h"

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

	MeshComp.mesh.Load("res\\models\\cube.obj");
	MeshComp.mesh.SetDefaultColor(glm::vec3(1.0f, 0.5f, 0.3f));
	MeshComp.mesh.FillRenderData(false, false, true);

	ShaderComponent& ShaderComp = reg.emplace<ShaderComponent>(cubes);
	ShaderComp.LoadShader("res\\shader\\basic3D.shader");

	Input& InputInstance = Input::Instance();
	InputInstance.SetCursorMode(CursorMode::CURSOR_DISABLED);

	// Start move
	CallbackHandlers[0] = InputInstance.BindKeyEvent(InputKey::KEY_W, InputMode::KEY_PRESS, [this]() { m_Dispatcher.enqueue<Event::MoveForward>(1.f); });
	CallbackHandlers[1] = InputInstance.BindKeyEvent(InputKey::KEY_S, InputMode::KEY_PRESS, [this]() { m_Dispatcher.enqueue<Event::MoveForward>(-1.f); });
	CallbackHandlers[2] = InputInstance.BindKeyEvent(InputKey::KEY_A, InputMode::KEY_PRESS, [this]() { m_Dispatcher.enqueue<Event::MoveRight>(-1.f); });
	CallbackHandlers[3] = InputInstance.BindKeyEvent(InputKey::KEY_D, InputMode::KEY_PRESS, [this]() { m_Dispatcher.enqueue<Event::MoveRight>(1.f); });
	// Stop move
	CallbackHandlers[4] = InputInstance.BindKeyEvent(InputKey::KEY_W, InputMode::KEY_RELEASE, [this]() { m_Dispatcher.enqueue<Event::MoveForward>(0.0f); });
	CallbackHandlers[5] = InputInstance.BindKeyEvent(InputKey::KEY_S, InputMode::KEY_RELEASE, [this]() { m_Dispatcher.enqueue<Event::MoveForward>(0.0f); });
	CallbackHandlers[6] = InputInstance.BindKeyEvent(InputKey::KEY_A, InputMode::KEY_RELEASE, [this]() { m_Dispatcher.enqueue<Event::MoveRight>(0.0f); });
	CallbackHandlers[7] = InputInstance.BindKeyEvent(InputKey::KEY_D, InputMode::KEY_RELEASE, [this]() { m_Dispatcher.enqueue<Event::MoveRight>(0.0f); });
	// Turn
	CallbackHandlers[8] = InputInstance.BindMouseMoveEvent([this](const glm::vec2 OldPos, const glm::vec2 NewPos) { m_Dispatcher.enqueue<Event::CursorMove>(OldPos, NewPos); });
	// Switch cursor mode
	CallbackHandlers[9] = InputInstance.BindKeyEvent(InputKey::KEY_TAB, InputMode::KEY_PRESS, [this]() {
		Input& InputInstance = Input::Instance();
		InputInstance.SetCursorMode(InputInstance.GetCursorMode() == CursorMode::CURSOR_DISABLED ? CursorMode::CURSOR_NORMAL : CursorMode::CURSOR_DISABLED);
		});
}

CameraMoveScene::~CameraMoveScene()
{
	for (InputCallbackHandler& Handler : CallbackHandlers)
	{
		Input::Instance().UnBindInputEvent(Handler);
	}
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
