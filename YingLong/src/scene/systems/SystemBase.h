#pragma once

#include "entt/entt.hpp"

#include "core/Macros.h"
#include "core/Log.h"
#include "scene/inputAction/InputActions.h"

DECLARE_LOGGER(SystemBaseLog)

namespace YingLong {

	class Scene;

	class SystemBase
	{
	public:
		void SetOwnerScene(const std::weak_ptr<Scene>& InOwnerScene)
		{
			auto SPtr = InOwnerScene.lock();
			Scene* OldScene = SPtr ? SPtr.get() : nullptr;
			OwnerScene = InOwnerScene;
			OnOwnerSceneChanged(OldScene);
		}

		SystemBase() {}
		virtual ~SystemBase() {}

		virtual void OnOwnerSceneChanged(Scene* OldScene) {}

		Scene& GetScene();

	protected:
		entt::dispatcher& GetDispatcher();
		entt::registry& GetRegistry();
		InputAction& GetInputAction();
		entt::entity& GetPrimaryCamera();

	private:
		std::weak_ptr<Scene> OwnerScene;
	};
}