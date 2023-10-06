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
		void OnMoveForward(float Value) { ForwardValue = Value; }
		void OnMoveRight(float Value) { RightValue = Value; }
		void OnTurnPitch(float Value) { TurnDirect.y = Value; }
		void OnTurnYaw(float Value) { TurnDirect.x = Value; }

	private:
		float ForwardValue = 0.f;
		float RightValue = 0.f;
		glm::vec2 TurnDirect{0.f, 0.f};
		InputAction::CallbackHandler InputActionHandlers[5];
	};
}