
#include "YingLongPCH.h"
#include "core/Macros.h"

#include "Input.h"

namespace YingLong {

	GLFWwindow* Input::m_Window = nullptr;

	InputCallbackHandler Input::m_Handler = 0;

	std::map<InputCallbackHandler, InputButtonCallback> Input::m_RegistedButtonCallbacks;
	std::map<InputCallbackHandler, InputMouseMoveCallback> Input::m_RegistedMouseMoveCallbacks;

	std::map<std::tuple<InputKey, InputMode>, std::vector<InputCallbackHandler>> Input::m_KeyModeToCallbacks;
	std::map<std::tuple<InputMouse, InputMode>, std::vector<InputCallbackHandler>> Input::m_MouseModeToCallbacks;

	std::vector<InputCallbackHandler> Input::m_MouseMoveCallbacks;
	std::map<InputMouse, std::vector<InputCallbackHandler>> Input::m_MouseMoveCallbacksWithMouse;

	std::map<InputKey, InputMode> Input::m_LastKeyMode;
	std::map<InputMouse, InputMode> Input::m_LastMouseMode;
	std::map<InputMouse, bool> Input::m_MousePressAndHold;


	void Input::InitInput(GLFWwindow* Window)
	{
		m_Window = Window;
		m_Handler = 0;
		m_RegistedButtonCallbacks.clear();
		m_RegistedMouseMoveCallbacks.clear();
		m_KeyModeToCallbacks.clear();
		m_MouseModeToCallbacks.clear();
		m_MouseMoveCallbacks.clear();
		m_MouseMoveCallbacksWithMouse.clear();
		m_LastKeyMode.clear();
		m_LastMouseMode.clear();
		m_MousePressAndHold.clear();
	}

	void Input::SetCursorMode(CursorMode Mode)
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, (int)Mode);
	}

	InputCallbackHandler Input::BindKeyEvent(InputKey Key, InputMode Mode, const InputButtonCallback& Callback, bool CheckUnique)
	{
		std::tuple<InputKey, InputMode> KeyTuple = { Key , Mode };
		if (m_KeyModeToCallbacks.find(KeyTuple) == m_KeyModeToCallbacks.end())
		{
			m_KeyModeToCallbacks.insert({ KeyTuple, std::vector<InputCallbackHandler>() });
		}
		auto& Callbacks = m_KeyModeToCallbacks[KeyTuple];

		// Check if Callback already registed for this Key-Mode
		if (CheckUnique)
		{
			for (const InputCallbackHandler& ExistCb : Callbacks)
			{
				if (Callback.target<void()>() == m_RegistedButtonCallbacks[ExistCb].target<void()>())
				{
					return ExistCb;
				}
			}
		}

		m_RegistedButtonCallbacks[++m_Handler] = Callback;
		Callbacks.push_back(m_Handler);

		// Init Key's mode if this key iis bind firt time
		const auto& LastKeyMode = m_LastKeyMode.find(Key);
		if (LastKeyMode == m_LastKeyMode.end())
		{
			m_LastKeyMode[Key] = (InputMode)glfwGetKey(m_Window, (int)Key);
		}

		return m_Handler;
	}

	InputCallbackHandler Input::BindMouseEvent(InputMouse Mouse, InputMode Mode, const InputButtonCallback& Callback, bool CheckUnique)
	{
		return false;
	}

	InputCallbackHandler Input::BindMouseMoveEvent(const InputMouseMoveCallback& Callback, bool CheckUnique)
	{
		return false;
	}

	InputCallbackHandler Input::BindMouseMoveEvent(InputMouse Mouse, const InputMouseMoveCallback& Callback, bool CheckUnique)
	{
		return false;
	}

	bool Input::UnBindInputEvent(InputCallbackHandler Handler)
	{
		return false;
	}

	bool Input::UnBindKeyEvent(InputKey Key, InputMode Mode, const InputCallback& Callback)
	{
		std::tuple<InputKey, InputMode> KeyTuple = { Key , Mode };
		const auto& FindResult = m_RegistedKeyCallback.find(KeyTuple);
		if (FindResult == m_RegistedKeyCallback.end())
		{
			return false;
		}
		auto& Callbacks = FindResult->second;
		for (auto Iter = Callbacks.begin(); Iter != Callbacks.end(); Iter++)
		{
			if (Callback.target<void()>() == (*Iter).target<void()>())
			{
				Callbacks.erase(Iter);
				return true;
			}
		}
		return false;
	}

	bool Input::IsKeyPressed(InputKey Key)
	{
		return glfwGetKey(m_Window, (int)Key) == GLFW_PRESS;
	}

	void Input::ProcessInput()
	{
		for (auto& Elem : m_RegistedKeyCallback)
		{
			InputKey Key = std::get<0>(Elem.first);
			InputMode Mode = std::get<1>(Elem.first);
			InputMode CurrentKeyMode = (InputMode)glfwGetKey(m_Window, (int)Key);

			const auto& LastKeyMode = m_LastKeyMode.find(Key);
			if (LastKeyMode != m_LastKeyMode.end() &&
				LastKeyMode->second == CurrentKeyMode)
			{
				continue;
			}

			if (CurrentKeyMode == Mode)
			{
				for (auto& Callback : Elem.second)
				{
					if (Callback)
					{
						Callback();
					}
				}
			}
		}

		for (auto& Elem : m_RegistedKeyCallback)
		{
			InputKey Key = std::get<0>(Elem.first);
			m_LastKeyMode[Key] = (InputMode)glfwGetKey(m_Window, (int)Key);
		}
	}
}