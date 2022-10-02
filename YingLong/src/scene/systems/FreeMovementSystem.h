#pragma once

#include "SystemBase.h"
#include "scene/events/EventTypes.h"

namespace YingLong {

	class FreeMovementSystem : public SystemBase
	{
	public:
		FreeMovementSystem();
		virtual ~FreeMovementSystem();

		virtual void OnOwnerSceneChanged(Scene* OldScene) override;

		void Update(float DeltaTime);

	private:
		void OnMoveForward(float Value) { m_ForwardValue = Value; }
		void OnMoveRight(float Value) { m_RightValue = Value; }
		void OnTurnPitch(float Value) { m_TurnDirect.y = Value; }
		void OnTurnYaw(float Value) { m_TurnDirect.x = Value; }

	private:
		float m_ForwardValue = 0.f;
		float m_RightValue = 0.f;
		glm::vec2 m_TurnDirect{0.f, 0.f};
		InputAction::CallbackHandler m_InputActionHandlers[4];
	};
}