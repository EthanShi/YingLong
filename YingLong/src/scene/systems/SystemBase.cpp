
#include "YingLongPCH.h"

#include "SystemBase.h"
#include "scene/Scene.h"

namespace YingLong {

	Scene& SystemBase::GetScene()
	{
		auto& SPtr = m_OwnerScene.lock();
		ASSERT(SPtr);
		return *SPtr;
	}

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

	InputAction& SystemBase::GetInputAction()
	{
		auto& SPtr = m_OwnerScene.lock();
		ASSERT(SPtr);
		return SPtr->GetInputAction();
	}

	entt::entity& SystemBase::GetPrimaryCamera()
	{
		auto& SPtr = m_OwnerScene.lock();
		ASSERT(SPtr);
		return SPtr->GetPrimaryCamera();
	}

}