#pragma once

#include <string>

#include "entt/entt.hpp"
#include "glm/glm.hpp"

#include "core/Macros.h"
#include "renderer/Camera3D.h"
#include "scene/inputAction/InputActions.h"

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

		entt::dispatcher& GetDispatcher() { return m_Dispatcher; }
		entt::registry& GetRegistry() { return m_Registry; }

		InputAction& GetInputAction() { return m_InputAction; }


	protected:
		virtual void Update(float deltatime);
		virtual void DrawEntities(float deltatime);
		virtual void DrawImgui(float deltatime);

		virtual void CreateDefaultCamera();

	protected:
		// Weak this ptr from owner
		std::weak_ptr<Scene> m_WeakThis;

		// Owner Engine
		std::weak_ptr<Engine> m_Engine;

		// Registry manage all entities & components in this Scene
		entt::registry m_Registry;

		// Event dispatcher manage all engine and user events for this Scene
		entt::dispatcher m_Dispatcher;

		// Clear color for renderer
		glm::vec4 m_BackgroundColor;

		// Camera that renderer use to render this Scene
		entt::entity m_PrimaryCamera;

		InputAction m_InputAction;
	};
}