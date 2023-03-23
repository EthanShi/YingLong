
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
}

CameraMoveScene::~CameraMoveScene()
{
	GetInputAction().ClearCallbacks();
}

void CameraMoveScene::OnActive(const std::shared_ptr<Engine>& OwnerEngine, const std::shared_ptr<Scene>& This)
{
	Scene::OnActive(OwnerEngine, This);

	CreateCubes();

	m_FreeMovementSystem.SetOwnerScene(This);
	m_DrawBasic3DMeshSystem.SetOwnerScene(This);
}

void CameraMoveScene::Update(float Deltatime)
{
	m_FreeMovementSystem.Update(Deltatime);
}

void CameraMoveScene::DrawImgui(float Deltatime)
{
}

void CameraMoveScene::DrawEntities(float deltatime)
{
	Scene::DrawEntities(deltatime);

	m_DrawBasic3DMeshSystem.Draw();
}

void CameraMoveScene::CreateDefaultCamera()
{
	Scene::CreateDefaultCamera();
	m_Registry.emplace<FreeMovementComponent>(m_PrimaryCamera);
}

void CameraMoveScene::CreateCubes()
{
	m_CubeMesh.LoadObjData("res/models/cube.obj");
	m_CubeMesh.SetDefaultColor(glm::vec3(1.f, 0.5f, 0.3f));
	m_CubeMesh.FillRenderData(true, true, true);

	glm::vec3 Scale(50.f, 50.f, 50.f);
	std::vector<glm::vec3> Positions = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(200.0f,  500.0f, -1500.0f),
		glm::vec3(-150.f, -220.f, -250.f),
		glm::vec3(-380.f, -20.f, -1230.f),
		glm::vec3(240.f, -40.f, -350.f),
		glm::vec3(-170.f,  300.f, -750.f),
		glm::vec3(130.f, -200.f, -250.f),
		glm::vec3(150.f,  200.f, -250.f),
		glm::vec3(150.f,  20.f, -150.f),
		glm::vec3(-130.f,  100.f, -150.f)
	};
	for (int i = Positions.size() - 1; i >= 0; i--)
	{
		CreateACube(Positions[i],
			glm::vec3(static_cast<double>(std::rand()) / RAND_MAX,
				static_cast<double>(std::rand()) / RAND_MAX,
				static_cast<double>(std::rand()) / RAND_MAX),
			Scale);
	}
}

void CameraMoveScene::CreateACube(const glm::vec3& Position, const glm::vec3& Forward, const glm::vec3& Scale)
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
	ShaderComp.LoadShader("../YingLong/res/shaders/basic3D.shader");
}
