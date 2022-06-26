
#include "YingLongPCH.h"

#include "SystemBase.h"
#include "scene/Scene.h"

namespace YingLong {

	entt::dispatcher& SystemBase::GetDispatcher()
	{
		return m_OwnerScene->GetDispatcher();
	}

	entt::registry& SystemBase::GetRegistry()
	{
		return m_OwnerScene->GetRegistry();
	}

}