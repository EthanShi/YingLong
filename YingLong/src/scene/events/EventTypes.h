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
	}
}