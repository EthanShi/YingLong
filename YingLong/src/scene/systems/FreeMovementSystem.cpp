
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

	FreeMovementSystem::~FreeMovementSystem()
	{
	}

	void FreeMovementSystem::OnOwnerSceneChanged(Scene* OldScene)
	{
		if (OldScene)
		{
			auto& OldInputAction = OldScene->GetInputAction();
			for (InputAction::CallbackHandler Handler : InputActionHandlers)
			{
				OldInputAction.UnBind(Handler);
			}
		}
		auto& InputAction = GetInputAction();

		InputActionHandlers[0] = InputAction.Bind("SwitchCursorMode", [this]() {
			Input& InputInstance = Input::Instance();
			InputInstance.SetCursorMode(InputInstance.GetCursorMode() == CursorMode::CURSOR_DISABLED ? CursorMode::CURSOR_NORMAL : CursorMode::CURSOR_DISABLED);
			});

		InputActionHandlers[1] = InputAction.Bind("MoveForward", INPUTACTION_AXIS_CALLBACK(this, OnMoveForward));
		InputActionHandlers[2] = InputAction.Bind("MoveRight", INPUTACTION_AXIS_CALLBACK(this, OnMoveRight));
		InputActionHandlers[3] = InputAction.Bind("Pitch", INPUTACTION_AXIS_CALLBACK(this, OnTurnPitch));
		InputActionHandlers[4] = InputAction.Bind("Yaw", INPUTACTION_AXIS_CALLBACK(this, OnTurnYaw));
	}

	void FreeMovementSystem::Update(float DeltaTime)
	{
		auto& reg = GetRegistry();
		auto view = reg.view<Transform3DComponent, FreeMovementComponent>();
		view.each(
			[this, DeltaTime](Transform3DComponent& Transform, const FreeMovementComponent& FreeMovement) {
				glm::vec3 Right = glm::cross(Transform.GetForward(), Transform.GetUp());
				glm::vec3 MoveDirection = Transform.GetForward() * ForwardValue + Right * RightValue;
				Transform.Move(MoveDirection * DeltaTime * FreeMovement.MoveSpeed);
				if (Input::Instance().GetCursorMode() == CursorMode::CURSOR_DISABLED)
				{
					glm::vec2 turnValue = TurnDirect * FreeMovement.TurnRate * DeltaTime;
					TurnDirect = { 0.f, 0.f };
					Transform.Rotate(-turnValue.y, -turnValue.x, 0.f);
				}
			}
		);
	}

}