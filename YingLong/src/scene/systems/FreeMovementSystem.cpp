
#include "YingLongPCH.h"

#include "FreeMovementSystem.h"
#include "scene/components/BasicComponents.h"
#include "scene/Scene.h"
#include "input/Input.h"

namespace YingLong {

	FreeMovementSystem::FreeMovementSystem()
		: SystemBase()
	{

	}

	void FreeMovementSystem::OnOwnerSceneChanged(Scene* OldScene)
	{
		if (OldScene)
		{
			auto& OldDispatcher = OldScene->GetDispatcher();
			OldDispatcher.disconnect(this);
		}
		auto& Dispatcher = GetDispatcher();
		Dispatcher.sink<Event::MoveForward>().connect<&FreeMovementSystem::OnMoveForward>(this);
		Dispatcher.sink<Event::MoveRight>().connect<&FreeMovementSystem::OnMoveRight>(this);
		Dispatcher.sink<Event::CursorMove>().connect<&FreeMovementSystem::OnCursorMove>(this);
	}

	void FreeMovementSystem::Update(float DeltaTime)
	{
		auto& reg = GetRegistry();
		auto& view = reg.view<Transform3DComponent, FreeMovementComponent>();
		view.each(
			[this, DeltaTime](Transform3DComponent& Transform, const FreeMovementComponent& FreeMovement) {
				glm::vec3 Right = glm::cross(Transform.GetForward(), Transform.GetUp());
				glm::vec3 MoveDirection = Transform.GetForward() * m_ForwardValue + Right * m_RightValue;
				Transform.Move(MoveDirection * DeltaTime * FreeMovement.MoveSpeed);
				if (Input::Instance().GetCursorMode() == CursorMode::CURSOR_DISABLED)
				{
					glm::vec2 turnValue = m_TurnDirect * FreeMovement.TurnRate * DeltaTime;
					m_TurnDirect = { 0.f, 0.f };
					Transform.Rotate(-turnValue.y, -turnValue.x, 0.f);
				}
			}
		);
	}

}