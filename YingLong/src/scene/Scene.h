#pragma once

#include <string>

#include "entt/entt.hpp"
#include "glm/glm.hpp"

#include "core/Macros.h"
#include "renderer/Camera3D.h"
#include "scene/inputAction/InputActions.h"

DECLARE_LOGGER(SceneLog)

namespace YingLong {

	class Engine;

	class YINGLONG_API Scene
	{
	public:
		Scene();
		virtual ~Scene();

		virtual void OnActive(const std::shared_ptr<Engine>& OwnerEngine, const std::shared_ptr<Scene>& This);
		virtual void OnInactive() {}

		virtual std::string GetName() = 0;

		void Tick(float deltatime);

		entt::dispatcher& GetDispatcher() { return Dispatcher; }
		entt::registry& GetRegistry() { return Registry; }

		InputAction& GetInputAction() { return InputAction; }

		entt::entity& GetPrimaryCamera() { return PrimaryCamera; }


	protected:
		virtual void Update(float deltatime);
		virtual void DrawEntities(float deltatime);
		virtual void DrawImgui(float deltatime);

		virtual void CreateDefaultCamera();

	protected:
		// Weak this ptr from owner
		std::weak_ptr<Scene> WeakThis;

		// Owner Engine
		std::weak_ptr<Engine> OwnerEngine;

		// Registry manage all entities & components in this Scene
		entt::registry Registry;

		// Event dispatcher manage all engine and user events for this Scene
		entt::dispatcher Dispatcher;

		// Camera that renderer use to render this Scene
		entt::entity PrimaryCamera;

		InputAction InputAction;
	};
}