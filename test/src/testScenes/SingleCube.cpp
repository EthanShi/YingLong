#include "SingleCube.h"
#include "renderer/Mesh.h"

#include "scene/components/BasicComponents.h"
#include "scene/components/DrawableComponents.h"

SingleCubeScene::SingleCubeScene()
	: Scene::Scene()
{
	auto& reg = Registry;

	// Init cubes mesh & shader
	const auto cubes = reg.create();
	Transform3DComponent& cubesTransform = reg.emplace<Transform3DComponent>(cubes);
	cubesTransform.SetScale(glm::vec3(100.f, 100.f, 100.f));
	cubesTransform.SetForward(glm::vec3(1.f, 1.f, 1.f));

	CubeMesh.LoadObjData("res/models/cube.obj");
	CubeMesh.SetDefaultColor(glm::vec3(1.0f, 0.5f, 0.3f));
	CubeMesh.FillRenderData(true, false, true);

	MeshComponent& MeshComp = reg.emplace<MeshComponent>(cubes, CubeMesh);

	ShaderComponent& ShaderComp = reg.emplace<ShaderComponent>(cubes);
	ShaderComp.LoadShader("../YingLong/res/shaders/basic3D.shader");
}

void SingleCubeScene::OnActive(const std::shared_ptr<Engine>& OwnerEngine, const std::shared_ptr<Scene>& This)
{
	Scene::OnActive(OwnerEngine, This);

	DrawBasic3DMeshSystem.SetOwnerScene(This);
}

void SingleCubeScene::DrawEntities(float deltatime)
{
	Scene::DrawEntities(deltatime);

	DrawBasic3DMeshSystem.Draw();
}
