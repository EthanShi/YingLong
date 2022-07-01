#pragma once

#include "SystemBase.h"
#include "scene/events/EventTypes.h"

namespace YingLong {

	class FreeMovementSystem : public SystemBase
	{
	public:
		FreeMovementSystem();
		virtual ~FreeMovementSystem() {}

		virtual void OnOwnerSceneChanged(Scene* OldScene) override;

		void Update(float DeltaTime);

	private:
		void OnMoveForward(const Event::MoveForward& Event) { m_ForwardValue = Event.Value; }
		void OnMoveRight(const Event::MoveRight& Event) { m_RightValue = Event.Value; }
		void OnCursorMove(const Event::CursorMove& Event) { m_TurnDirect = Event.NewPos - Event.OldPos; }

	private:
		float m_ForwardValue = 0.f;
		float m_RightValue = 0.f;
		glm::vec2 m_TurnDirect{0.f, 0.f};
	};
}