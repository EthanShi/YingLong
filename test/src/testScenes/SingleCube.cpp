#include "SingleCube.h"
#include "renderer/Mesh.h"

#include "scene/components/BasicComponents.h"
#include "scene/components/DrawableComponents.h"

SingleCubeScene::SingleCubeScene()
	: Scene::Scene()
{
	auto& reg = GetRegistry();

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
}

void SingleCubeScene::Update(float deltatime)
{

}
