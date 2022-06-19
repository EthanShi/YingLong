#pragma once

#include "GLFW/glfw3.h"

#include "core/Macros.h"
#include "InputTypes.h"

namespace YingLong {

	using InputCallback = std::function<void()>;

	class Input
	{
	private:
		static GLFWwindow* m_Window;

		static std::map<std::tuple<InputKey, InputKeyMode>, std::vector<InputCallback>> m_RegistedKeyCallback;
		static std::map<InputKey, InputKeyMode> m_LastKeyMode;

	public:
		static void InitInput(GLFWwindow* Window);

		static bool BindKeyEvent(InputKey Key, InputKeyMode Mode, const InputCallback& Callback);
		static bool UnBindKeyEvent(InputKey Key, InputKeyMode Mode, const InputCallback& Callback);

		static bool IsKeyPressed(InputKey Key);

		static void ProcessInput();
	};

}
