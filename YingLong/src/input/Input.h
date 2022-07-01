#pragma once

#include "GLFW/glfw3.h"

#include "core/Macros.h"
#include "utils/Hash.h"
#include "InputTypes.h"

namespace YingLong {

	class Input
	{
	public:
		static Input& Instance()
		{
			static Input instance;
			return instance;
		}

		void InitInput(GLFWwindow* Window);

		void SetCursorMode(CursorMode Mode);
		CursorMode GetCursorMode();

		/* Callback will be called each time Key's mode change to Mode. */
		InputCallbackHandler BindKeyEvent(InputKey Key, InputMode Mode, const InputButtonCallback& Callback, bool CheckUnique = false);

		/* Callback will be called each time Mouse's mode change to Mode. */
		InputCallbackHandler BindMouseEvent(InputMouse Mouse, InputMode Mode, const InputButtonCallback& Callback, bool CheckUnique = false);

		/* Callback will be called each time mouse move. */
		InputCallbackHandler BindMouseMoveEvent(const InputMouseMoveCallback& Callback, bool CheckUnique = false);

		/* Callback will be called each time mouse move with Mouse button press and hold. */
		InputCallbackHandler BindMouseMoveEvent(InputMouse Mouse, const InputMouseMoveCallback& Callback, bool CheckUnique = false);

		void UnBindInputEvent(const InputCallbackHandler& Handler);

		bool IsKeyPressed(InputKey Key);

		void ProcessInput();

	private:
		GLFWwindow* m_Window = nullptr;

		InputCallbackHandler m_Handler = 0;

		/* All registed callbacks in input system. */
		std::unordered_map<InputCallbackHandler, InputButtonCallback> m_RegistedButtonCallbacks;
		std::unordered_map<InputCallbackHandler, InputMouseMoveCallback> m_RegistedMouseMoveCallbacks;

		/* Helper data to determine which callback should be called. */
		std::unordered_map<std::pair<InputKey, InputMode>, std::vector<InputCallbackHandler>, HashPair> m_KeyModeToCallbacks;
		std::unordered_map<std::pair<InputMouse, InputMode>, std::vector<InputCallbackHandler>, HashPair> m_MouseModeToCallbacks;

		std::vector<InputCallbackHandler> m_MouseMoveCallbacks;
		std::unordered_map<InputMouse, std::vector<InputCallbackHandler>> m_MouseMoveCallbacksWithMouse;

		std::unordered_map<InputKey, InputMode> m_LastKeyMode;
		std::unordered_map<InputMouse, InputMode> m_LastMouseMode;
		glm::dvec2 m_LastCursorPos {0.f, 0.f};

	private:
		Input() {}
		~Input() {}
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;
	};
}
