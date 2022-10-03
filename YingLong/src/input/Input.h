#pragma once

#include "core/Macros.h"
#include "utils/Hash.h"
#include "InputTypes.h"

struct GLFWwindow;

namespace YingLong {

	class YINGLONG_API Input
	{
	public:

		using CallbackHandler = uint64;

		using KeyChangeCallback = std::function<void(InputKey, InputMode)>;
		using MouseChangeCallback = std::function<void(InputMouse, InputMode)>;
		using InputButtonCallback = std::function<void()>;
		using InputMouseMoveCallback = std::function<void(const glm::dvec2&, const glm::dvec2&)>;

		static Input& Instance()
		{
			static Input instance;
			return instance;
		}

		void InitInput(GLFWwindow* Window);

		void SetCursorMode(CursorMode Mode);
		CursorMode GetCursorMode();

		void AddCareKeys(std::vector<InputKey> Keys);
		void AddCareMouses(std::vector<InputMouse> Mouses);

		/* Callback will be called any key mode change. May be called many times per frame. */
		CallbackHandler BindKeyEvent(const KeyChangeCallback& Callback);

		/* Callback will be called each time Key's mode change to Mode. Will add to care keys. */
		CallbackHandler BindKeyEvent(InputKey Key, InputMode Mode, const InputButtonCallback& Callback, bool CheckUnique = false);
		
		/* Callback will be called any mouse mode change. May be called many times per frame. */
		CallbackHandler BindMouseEvent(const MouseChangeCallback& Callback);

		/* Callback will be called each time Mouse's mode change to Mode. Will add to care mouses. */
		CallbackHandler BindMouseEvent(InputMouse Mouse, InputMode Mode, const InputButtonCallback& Callback, bool CheckUnique = false);

		/* Callback will be called each time mouse move. */
		CallbackHandler BindMouseMoveEvent(const InputMouseMoveCallback& Callback, bool CheckUnique = false);

		/* Callback will be called each time mouse move with Mouse button press and hold. */
		CallbackHandler BindMouseMoveEvent(InputMouse Mouse, const InputMouseMoveCallback& Callback, bool CheckUnique = false);

		void UnBindInputEvent(const CallbackHandler& Handler);

		bool IsKeyPressed(InputKey Key);

		void ProcessInput();

	private:
		GLFWwindow* m_Window = nullptr;

		CallbackHandler m_Handler = 0;

		/* All registed callbacks in input system. */
		std::unordered_map<CallbackHandler, InputButtonCallback> m_RegistedButtonCallbacks;
		std::unordered_map<CallbackHandler, InputMouseMoveCallback> m_RegistedMouseMoveCallbacks;
		std::unordered_map<CallbackHandler, KeyChangeCallback>  m_RegistedKeyChangeCallbacks;
		std::unordered_map<CallbackHandler, MouseChangeCallback> m_RegistedMouseChangeCallbacks;

		/* Helper data to determine which callback should be called. */
		std::unordered_map<std::pair<InputKey, InputMode>, std::vector<CallbackHandler>, HashPair> m_KeyModeToCallbacks;
		std::unordered_map<std::pair<InputMouse, InputMode>, std::vector<CallbackHandler>, HashPair> m_MouseModeToCallbacks;

		std::vector<CallbackHandler> m_MouseMoveCallbacks;
		std::unordered_map<InputMouse, std::vector<CallbackHandler>> m_MouseMoveCallbacksWithMouse;

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
