
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
	Input& InputInstance = Input::Instance();
	InputInstance.SetCursorMode(CursorMode::CURSOR_DISABLED);

	glm::vec3 Scale(50.f, 50.f, 50.f);
	glm::vec3 Color(1.f, 0.5f, 0.3f);
	std::vector<glm::vec3> Positions = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	for (int i = Positions.size() - 1; i >= 0; i--)
	{
		CreateACube(Positions[i],
			glm::vec3(static_cast<double>(std::rand()) / RAND_MAX,
				static_cast<double>(std::rand()) / RAND_MAX,
				static_cast<double>(std::rand()) / RAND_MAX),
			Scale, Color);
	}

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

void CameraMoveScene::CreateACube(const glm::vec3& Position, const glm::vec3& Forward, const glm::vec3& Scale, const glm::vec3& Color)
{
	auto& reg = m_Registry;

	// Init cubes mesh & shader
	const auto cube = reg.create();
	Transform3DComponent& cubesTranform = reg.emplace<Transform3DComponent>(cube);
	cubesTranform.SetPosition(Position);
	cubesTranform.SetScale(Scale);
	cubesTranform.SetForward(Forward);

	MeshComponent& MeshComp = reg.emplace<MeshComponent>(cube);

	MeshComp.mesh.Load("res/models/cube.obj");
	MeshComp.mesh.SetDefaultColor(Color);
	MeshComp.mesh.FillRenderData(false, false, true);

	ShaderComponent& ShaderComp = reg.emplace<ShaderComponent>(cube);
	ShaderComp.LoadShader("res/shader/basic3D.shader");
}
