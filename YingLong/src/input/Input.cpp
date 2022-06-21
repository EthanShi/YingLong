
#include "YingLongPCH.h"
#include "core/Macros.h"

#include "Input.h"

namespace YingLong {

	GLFWwindow* Input::m_Window = nullptr;

	InputCallbackHandler Input::m_Handler = 0;

	std::unordered_map<InputCallbackHandler, InputButtonCallback> Input::m_RegistedButtonCallbacks;
	std::unordered_map<InputCallbackHandler, InputMouseMoveCallback> Input::m_RegistedMouseMoveCallbacks;

	std::unordered_map<std::tuple<InputKey, InputMode>, std::vector<InputCallbackHandler>> Input::m_KeyModeToCallbacks;
	std::unordered_map<std::tuple<InputMouse, InputMode>, std::vector<InputCallbackHandler>> Input::m_MouseModeToCallbacks;

	std::vector<InputCallbackHandler> Input::m_MouseMoveCallbacks;
	std::unordered_map<InputMouse, std::vector<InputCallbackHandler>> Input::m_MouseMoveCallbacksWithMouse;

	std::unordered_map<InputKey, InputMode> Input::m_LastKeyMode;
	std::unordered_map<InputMouse, InputMode> Input::m_LastMouseMode;
	std::unordered_map<InputMouse, bool> Input::m_MousePressAndHold;
	glm::dvec2 Input::m_LastCursorPos;


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
		m_LastCursorPos.x = 0.f;
		m_LastCursorPos.y = 0.f;
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

		// Init Key's mode if this key is bind firt time
		const auto& LastKeyMode = m_LastKeyMode.find(Key);
		if (LastKeyMode == m_LastKeyMode.end())
		{
			m_LastKeyMode[Key] = (InputMode)glfwGetKey(m_Window, (int)Key);
		}

		return m_Handler;
	}

	InputCallbackHandler Input::BindMouseEvent(InputMouse Mouse, InputMode Mode, const InputButtonCallback& Callback, bool CheckUnique)
	{
		std::tuple<InputMouse, InputMode> KeyTuple = { Mouse , Mode };
		if (m_MouseModeToCallbacks.find(KeyTuple) == m_MouseModeToCallbacks.end())
		{
			m_MouseModeToCallbacks.insert({ KeyTuple, std::vector<InputCallbackHandler>() });
		}
		auto& Callbacks = m_MouseModeToCallbacks[KeyTuple];

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
		
		// Init Mouse's mode if this key is bind firt time
		const auto& LastMouseMode = m_LastMouseMode.find(Mouse);
		if (LastMouseMode == m_LastMouseMode.end())
		{
			m_LastMouseMode[Mouse] = (InputMode)glfwGetMouseButton(m_Window, (int)Mouse);
		}

		return m_Handler;
	}

	InputCallbackHandler Input::BindMouseMoveEvent(const InputMouseMoveCallback& Callback, bool CheckUnique)
	{
		if (CheckUnique)
		{
			for (auto& ExistCb : m_RegistedMouseMoveCallbacks)
			{
				if (Callback.target<void()>() == ExistCb.second.target<void()>())
				{
					return ExistCb.first;
				}
			}
		}

		m_RegistedMouseMoveCallbacks[++m_Handler] = Callback;
		m_MouseMoveCallbacks.push_back(m_Handler);

		return m_Handler;
	}

	InputCallbackHandler Input::BindMouseMoveEvent(InputMouse Mouse, const InputMouseMoveCallback& Callback, bool CheckUnique)
	{
		if (m_MouseMoveCallbacksWithMouse.find(Mouse) == m_MouseMoveCallbacksWithMouse.end())
		{
			m_MouseMoveCallbacksWithMouse.insert({ Mouse, std::vector<InputCallbackHandler>() });
		}

		if (CheckUnique)
		{
			for (auto& ExistCb : m_RegistedMouseMoveCallbacks)
			{
				if (Callback.target<void()>() == ExistCb.second.target<void()>())
				{
					return ExistCb.first;
				}
			}
		}

		m_RegistedMouseMoveCallbacks[++m_Handler] = Callback;
		m_MouseMoveCallbacksWithMouse[Mouse].push_back(m_Handler);

		return m_Handler;
	}

	bool Input::UnBindInputEvent(const InputCallbackHandler& Handler)
	{
		m_RegistedButtonCallbacks.erase(Handler);
		m_RegistedMouseMoveCallbacks.erase(Handler);

		for (auto& Iter : m_KeyModeToCallbacks)
		{
			Iter.second.erase(std::remove(Iter.second.begin(), Iter.second.end(), Handler));
		}
		for (auto& Iter : m_MouseModeToCallbacks)
		{
			Iter.second.erase(std::remove(Iter.second.begin(), Iter.second.end(), Handler));
		}

		m_MouseMoveCallbacks.erase(std::remove(m_MouseMoveCallbacks.begin(), m_MouseMoveCallbacks.end(), Handler));
		for (auto& Iter : m_MouseMoveCallbacksWithMouse)
		{
			Iter.second.erase(std::remove(Iter.second.begin(), Iter.second.end(), Handler));
		}
	}

	bool Input::IsKeyPressed(InputKey Key)
	{
		return glfwGetKey(m_Window, (int)Key) == GLFW_PRESS;
	}

	void Input::ProcessInput()
	{
		std::unordered_map<InputKey, InputMode> ChangedKeys;
		for (auto& Iter : m_KeyModeToCallbacks)
		{
			InputKey Key = std::get<0>(Iter.first);
			InputMode Mode = std::get<1>(Iter.first);
			InputMode CurrentKeyMode = (InputMode)glfwGetKey(m_Window, (int)Key);

			const auto& LastKeyMode = m_LastKeyMode.find(Key);
			if (LastKeyMode != m_LastKeyMode.end() &&
				LastKeyMode->second == CurrentKeyMode)
			{
				continue;
			}

			ChangedKeys.insert({ Key, CurrentKeyMode });
			m_LastKeyMode[Key] = CurrentKeyMode;
		}

		std::unordered_map<InputMouse, InputMode> ChangedMouses;
		for (auto& Iter : m_MouseModeToCallbacks)
		{
			InputMouse Mouse = std::get<0>(Iter.first);
			InputMode Mode = std::get<1>(Iter.first);
			InputMode CurrentMouseMode = (InputMode)glfwGetKey(m_Window, (int)Mouse);

			const auto& LastMouseMode = m_LastMouseMode.find(Mouse);
			if (LastMouseMode != m_LastMouseMode.end() &&
				LastMouseMode->second == CurrentMouseMode)
			{
				continue;
			}

			ChangedMouses.insert({ Mouse, CurrentMouseMode });
			m_LastMouseMode[Mouse] = CurrentMouseMode;
		}

		double CursorX, CursorY;
		glfwGetCursorPos(m_Window, &CursorX, &CursorY);
		glm::vec2 CurrentCursorPos(CursorX, CursorY);
		if (DOUBLE_EQUAL(m_LastCursorPos.x, 0.f) && DOUBLE_EQUAL(m_LastCursorPos.y, 0.f))
		{
			m_LastCursorPos = CurrentCursorPos;
		}

		// TODO: call callbacks
	}
}