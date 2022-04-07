#pragma once

#include "GLFW/glfw3.h"

#include "core/Macros.h"

namespace YingLong {

	using InputCallback = std::function<void()>;

	class Input
	{
	private:
		static GLFWwindow* m_Window;

		static std::map<std::tuple<int32, int32>, std::vector<InputCallback>> m_RegistedKeyCallback;
		static std::map<int32, int32> m_LastKeyMode;

	public:
		static void InitInput(GLFWwindow* Window);

		static bool BindKeyEvent(int32 InputKey, int32 InputMode, const InputCallback& Callback);
		static bool UnBindKeyEvent(int32 InputKey, int32 InputMode, const InputCallback& Callback);

		static bool IsKeyPressed(int32 Key);

		static void ProcessInput();
	};

}
