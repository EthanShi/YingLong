#pragma once

#include "entt/entt.hpp"

#include "core/Macros.h"

namespace YingLong {

	class Scene;

	class SystemBase
	{
	public:
		void SetOwnerScene(const std::weak_ptr<Scene>& OwnerScene)
		{
			auto& SPtr = m_OwnerScene.lock();
			Scene* OldScene = SPtr ? SPtr.get() : nullptr;
			m_OwnerScene = OwnerScene;
			OnOwnerSceneChanged(OldScene);
		}

		SystemBase() {}
		virtual ~SystemBase() {}

		virtual void OnOwnerSceneChanged(Scene* OldScene) {}

		Scene& GetScene()
		{
			auto& SPtr = m_OwnerScene.lock();
			ASSERT(SPtr);
			return *SPtr;
		}

	protected:
		entt::dispatcher& GetDispatcher();
		entt::registry& GetRegistry();

	private:
		std::weak_ptr<Scene> m_OwnerScene;
	};
}