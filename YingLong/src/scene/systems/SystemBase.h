#pragma once

#include "entt/entt.hpp"

#include "scene/SceneTypes.h"

namespace YingLong {

	class Scene;

	class SystemBase
	{
	public:
		SystemBase(Scene_SPtr OwnerScene)
		{
			m_OwnerScene = OwnerScene;
		}

		virtual ~SystemBase() {}

		Scene& GetScene()
		{
			return *m_OwnerScene;
		}

	protected:
		entt::dispatcher& GetDispatcher();
		entt::registry& GetRegistry();

	private:
		Scene_SPtr m_OwnerScene;
	};
}