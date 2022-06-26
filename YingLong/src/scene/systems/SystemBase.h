#pragma once

#include <memory>

namespace YingLong {

	class Scene;

	class SystemBase
	{
		SystemBase(std::shared_ptr<Scene> OwnerScene)
		{
			m_OwnerScene = OwnerScene;
		}

		~SystemBase() {}

		Scene& GetScene()
		{
			return *m_OwnerScene;
		}

	private:
		std::shared_ptr<Scene> m_OwnerScene;
	};

}