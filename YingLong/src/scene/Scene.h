#pragma once

#include "entt/entt.hpp"
#include "glm/glm.hpp"

#include "core/Macros.h"
#include "renderer/Camera3D.h"

namespace YingLong {

	class YINGLONG_API Scene
	{
	public:
		Scene();
		virtual ~Scene();

		void Tick(float deltatime);

		entt::dispatcher& GetDispatcher() { return m_Dispatcher; }

	protected:
		virtual void Update(float deltatime);
		virtual void DrawEntities(float deltatime);
		virtual void DrawImgui(float deltatime);

		virtual void CreateDefaultCamera();

	protected:
		// Registry manage all entities & components in this Scene
		entt::registry m_Registry;

		// Event dispatcher manage all engine and user events for this Scene
		entt::dispatcher m_Dispatcher;

		// Clear color for renderer
		glm::vec4 m_BackgroundColor;

		// Camera that renderer use to render this Scene
		entt::entity m_PrimaryCamera;
	};

	using Scene_SPtr = std::shared_ptr<Scene>;
}