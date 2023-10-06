
#include "YingLongPCH.h"

#include "SystemBase.h"
#include "scene/Scene.h"

DEFINE_LOGGER(SystemBaseLog)

namespace YingLong {

	Scene& SystemBase::GetScene()
	{
		auto SPtr = OwnerScene.lock();
		ASSERT(SPtr);
		return *SPtr;
	}

	entt::dispatcher& SystemBase::GetDispatcher()
	{
		auto SPtr = OwnerScene.lock();
		ASSERT(SPtr);
		return SPtr->GetDispatcher();
	}

	entt::registry& SystemBase::GetRegistry()
	{
		auto SPtr = OwnerScene.lock();
		ASSERT(SPtr);
		return SPtr->GetRegistry();
	}

	InputAction& SystemBase::GetInputAction()
	{
		auto SPtr = OwnerScene.lock();
		ASSERT(SPtr);
		return SPtr->GetInputAction();
	}

	entt::entity& SystemBase::GetPrimaryCamera()
	{
		auto SPtr = OwnerScene.lock();
		ASSERT(SPtr);
		return SPtr->GetPrimaryCamera();
	}

}