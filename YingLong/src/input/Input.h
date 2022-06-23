#pragma once

#include "GLFW/glfw3.h"
#include "boost/container_hash/hash.hpp"

#include "core/Macros.h"
#include "InputTypes.h"

namespace YingLong {

	class Input
	{
	public:
		static void InitInput(GLFWwindow* Window);

		static void SetCursorMode(CursorMode Mode);

		/* Callback will be called each time Key's mode change to Mode. */
		static InputCallbackHandler BindKeyEvent(InputKey Key, InputMode Mode, const InputButtonCallback& Callback, bool CheckUnique = false);

		/* Callback will be called each time Mouse's mode change to Mode. */
		static InputCallbackHandler BindMouseEvent(InputMouse Mouse, InputMode Mode, const InputButtonCallback& Callback, bool CheckUnique = false);

		/* Callback will be called each time mouse move. */
		static InputCallbackHandler BindMouseMoveEvent(const InputMouseMoveCallback& Callback, bool CheckUnique = false);

		/* Callback will be called each time mouse move with Mouse button press and hold. */
		static InputCallbackHandler BindMouseMoveEvent(InputMouse Mouse, const InputMouseMoveCallback& Callback, bool CheckUnique = false);

		static void UnBindInputEvent(const InputCallbackHandler& Handler);

		static bool IsKeyPressed(InputKey Key);

		static void ProcessInput();

	private:
		static GLFWwindow* m_Window;

		static InputCallbackHandler m_Handler;

		/* All registed callbacks in input system. */
		static std::unordered_map<InputCallbackHandler, InputButtonCallback> m_RegistedButtonCallbacks;
		static std::unordered_map<InputCallbackHandler, InputMouseMoveCallback> m_RegistedMouseMoveCallbacks;

		/* Helper data to determine which callback should be called. */
		static std::unordered_map<std::pair<InputKey, InputMode>, std::vector<InputCallbackHandler>, boost::hash<std::pair<InputKey, InputMode>>> m_KeyModeToCallbacks;
		static std::unordered_map<std::pair<InputMouse, InputMode>, std::vector<InputCallbackHandler>, boost::hash<std::pair<InputMouse, InputMode>>> m_MouseModeToCallbacks;

		static std::vector<InputCallbackHandler> m_MouseMoveCallbacks;
		static std::unordered_map<InputMouse, std::vector<InputCallbackHandler>> m_MouseMoveCallbacksWithMouse;

		static std::unordered_map<InputKey, InputMode> m_LastKeyMode;
		static std::unordered_map<InputMouse, InputMode> m_LastMouseMode;
		static glm::dvec2 m_LastCursorPos;
	};
}
