#pragma once

#include <string>

#include "glm/glm.hpp"

#include "input/InputTypes.h"

namespace YingLong {

	namespace Event {
		
		template<InputKey Key>
		struct KeyPress {};

		template<InputKey Key>
		struct KeyRelease {};

		template<InputKey Mouse>
		struct MousePress {};

		template<InputKey Mouse>
		struct MouseRelease {};

		struct CursorMove
		{
			glm::vec2 OldPos;
			glm::vec2 NewPos;
		};

		struct MoveForward
		{
			float Value;	// [-1, 1]
		};

		struct MoveRight
		{
			float Value;	// [-1, 1]
		};

		template<uint32 ActionNumber>
		struct TriggerInputAction {};

		template<uint32 ActionNumber>
		struct AxisInputAction
		{
			float Value;	// [-1, 1]
		};
	}
}