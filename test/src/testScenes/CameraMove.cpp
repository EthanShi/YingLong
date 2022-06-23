#include "CameraMove.h"

#include "YingLongPCH.h"

#include "glad/gl.h"

#include "input/Input.h"

#include "scene/components/BasicComponents.h"
#include "scene/components/DrawableComponents.h"


CameraMoveScene::CameraMoveScene()
	: Scene::Scene()
{
	auto& reg = GetRegistry();

	// Init cubes mesh & shader
	const auto cubes = reg.create();
	Transform3DComponent& cubesTransform = reg.emplace<Transform3DComponent>(cubes);
	cubesTransform.Scale = glm::vec3(100.f, 100.f, 100.f);
	cubesTransform.Forward = glm::vec3(1.f, 1.f, 1.f);

	MeshComponent& MeshComp = reg.emplace<MeshComponent>(cubes);

	MeshComp.mesh.Load("res\\models\\cube.obj");
	MeshComp.mesh.SetDefaultColor(glm::vec3(1.0f, 0.5f, 0.3f));
	MeshComp.mesh.FillRenderData(false, false, true);

	ShaderComponent& ShaderComp = reg.emplace<ShaderComponent>(cubes);
	ShaderComp.LoadShader("res\\shader\\basic3D.shader");

	// Start move
	CallbackHandlers[0] = Input::BindKeyEvent(InputKey::KEY_W, InputMode::KEY_PRESS, [this]() { MoveCameraForward(1.0f); });
	CallbackHandlers[1] = Input::BindKeyEvent(InputKey::KEY_S, InputMode::KEY_PRESS, [this]() { MoveCameraForward(-1.0f); });
	CallbackHandlers[2] = Input::BindKeyEvent(InputKey::KEY_A, InputMode::KEY_PRESS, [this]() { MoveCameraRight(-1.0f); });
	CallbackHandlers[3] = Input::BindKeyEvent(InputKey::KEY_D, InputMode::KEY_PRESS, [this]() { MoveCameraRight(1.0f); });
	// Stop move
	CallbackHandlers[4] = Input::BindKeyEvent(InputKey::KEY_W, InputMode::KEY_RELEASE, [this]() { MoveCameraForward(0.0f); });
	CallbackHandlers[5] = Input::BindKeyEvent(InputKey::KEY_S, InputMode::KEY_RELEASE, [this]() { MoveCameraForward(0.0f); });
	CallbackHandlers[6] = Input::BindKeyEvent(InputKey::KEY_A, InputMode::KEY_RELEASE, [this]() { MoveCameraRight(0.0f); });
	CallbackHandlers[7] = Input::BindKeyEvent(InputKey::KEY_D, InputMode::KEY_RELEASE, [this]() { MoveCameraRight(0.0f); });
	// Turn
	CallbackHandlers[8] = Input::BindMouseMoveEvent(std::bind(&CameraMoveScene::TurnCamera, this, std::placeholders::_1, std::placeholders::_2));
}

void CameraMoveScene::Update(float Deltatime)
{
	auto& reg = GetRegistry();
	Transform3DComponent& CameraTransform = reg.get<Transform3DComponent>(GetPrimaryCamera());
	glm::vec3 Right = glm::cross(CameraTransform.Forward.Value(), CameraTransform.Up.Value());
	glm::vec3 MoveDirection = CameraTransform.Forward.Value() * m_MoveForwardValue + Right * m_MoveRightValue;
	CameraTransform.Position += MoveDirection * Deltatime * m_MoveSpeed;
}

void CameraMoveScene::DrawImgui(float Deltatime)
{
}

void CameraMoveScene::MoveCameraForward(float Value)
{
	m_MoveForwardValue = Value;
}

void CameraMoveScene::MoveCameraRight(float Value)
{
	m_MoveRightValue = Value;
}

void CameraMoveScene::TurnCamera(const glm::dvec2& OldPos, const glm::dvec2& NewPos)
{
	std::cout << "turn" << std::endl;
}
