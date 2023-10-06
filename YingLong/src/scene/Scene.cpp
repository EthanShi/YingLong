
#include "YingLongPCH.h"

#include "core/Engine.h"
#include "core/Config.h"
#include "Scene.h"
#include "renderer/Renderer3D.h"

#include "components/BasicComponents.h"
#include "components/DrawableComponents.h"

DEFINE_LOGGER(SceneLog)

namespace YingLong {

	Scene::Scene()
		: Registry()
		, Dispatcher()
		, PrimaryCamera()
		, InputAction()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::OnActive(const std::shared_ptr<Engine>& InOwnerEngine, const std::shared_ptr<Scene>& This)
	{
		OwnerEngine = InOwnerEngine;
		WeakThis = This;

		Renderer::SetDepthTestEnable(true);

		CreateDefaultCamera();

		// Init configs
		Config& ConfigInstance = Config::Instance();
		ConfigInstance.LoadSceneConfig(Config::ConfigLayer::Scene, GetName());
		ConfigInstance.LoadSceneConfig(Config::ConfigLayer::SceneUser, GetName());

		InputAction.Init(WeakThis);

		std::stringstream StrStream;
		StrStream << "Config with user:";
		StrStream << toml::json_formatter(ConfigInstance.ReadOnlyWithUser());
		StrStream << "Config without user:";
		StrStream << toml::json_formatter(ConfigInstance.ReadOnly());

		SceneLog().info(StrStream.str());
	}

	void Scene::Tick(float deltatime)
	{
		Dispatcher.update();
		Update(deltatime);
		DrawEntities(deltatime);
		DrawImgui(deltatime);
	}

	void Scene::Update(float deltatime)
	{
	}

	void Scene::DrawEntities(float deltatime)
	{
	}

	void Scene::DrawImgui(float deltatime)
	{
	}

	void Scene::CreateDefaultCamera()
	{
		PrimaryCamera = Registry.create();
		Transform3DComponent& cameraTransform = Registry.emplace<Transform3DComponent>(PrimaryCamera);
		cameraTransform.SetPosition(glm::vec3(0.0f, 0.0f, 500.0f));
		Registry.emplace<Camera3DComponent>(PrimaryCamera);
	}
}