
#include "YingLongPCH.h"

#include "SystemBase.h"
#include "scene/Scene.h"

namespace YingLong {

	entt::dispatcher& SystemBase::GetDispatcher()
	{
		auto& SPtr = m_OwnerScene.lock();
		ASSERT(SPtr);
		return SPtr->GetDispatcher();
	}

	entt::registry& SystemBase::GetRegistry()
	{
		auto& SPtr = m_OwnerScene.lock();
		ASSERT(SPtr);
		return SPtr->GetRegistry();
	}

}