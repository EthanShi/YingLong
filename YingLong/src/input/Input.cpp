
#include "YingLongPCH.h"

#include "core/Macros.h"

#include "Input.h"

namespace YingLong {

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
		m_LastCursorPos = { 0.f, 0.f };
	}

	void Input::SetCursorMode(CursorMode Mode)
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, (int)Mode);
	}

	CursorMode Input::GetCursorMode()
	{
		return (CursorMode)glfwGetInputMode(m_Window, GLFW_CURSOR);
	}

	InputCallbackHandler Input::BindKeyEvent(InputKey Key, InputMode Mode, const InputButtonCallback& Callback, bool CheckUnique)
	{
		std::pair<InputKey, InputMode> KeyTuple = { Key , Mode };
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
		std::pair<InputMouse, InputMode> KeyTuple = { Mouse , Mode };
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

	void Input::UnBindInputEvent(const InputCallbackHandler& Handler)
	{
		m_RegistedButtonCallbacks.erase(Handler);
		m_RegistedMouseMoveCallbacks.erase(Handler);

		auto& RemoveHandlerFromVector = [](std::vector<InputCallbackHandler>& Vector, const InputCallbackHandler& Handler)
		{
			auto& WhereIter = std::remove(Vector.begin(), Vector.end(), Handler);
			if (WhereIter != Vector.end())
			{
				Vector.erase(WhereIter);
			}
		};

		for (auto& Iter : m_KeyModeToCallbacks)
		{
			RemoveHandlerFromVector(Iter.second, Handler);
		}
		for (auto& Iter : m_MouseModeToCallbacks)
		{
			RemoveHandlerFromVector(Iter.second, Handler);
		}

		RemoveHandlerFromVector(m_MouseMoveCallbacks, Handler);
		for (auto& Iter : m_MouseMoveCallbacksWithMouse)
		{
			RemoveHandlerFromVector(Iter.second, Handler);
		}
	}

	bool Input::IsKeyPressed(InputKey Key)
	{
		return glfwGetKey(m_Window, (int)Key) == GLFW_PRESS;
	}

	void Input::ProcessInput()
	{
		// Gather changed keys
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
			m_LastKeyMode[Key] = CurrentKeyMode;	// All modes in m_LastKeyMode updated
		}

		// Gather changed mouses
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

		// Update mouse state for mouse move
		for (auto& Iter : m_MouseMoveCallbacksWithMouse)
		{
			m_LastMouseMode[Iter.first] = (InputMode)glfwGetKey(m_Window, (int)Iter.first);	// All modes in m_LastMouseMode updated
		}

		// Get current cursor position
		double CursorX, CursorY;
		glfwGetCursorPos(m_Window, &CursorX, &CursorY);
		glm::dvec2 CurrentCursorPos(CursorX, CursorY);
		if (DOUBLE_EQUAL(m_LastCursorPos.x, 0.f) && DOUBLE_EQUAL(m_LastCursorPos.y, 0.f))
		{
			m_LastCursorPos = CurrentCursorPos;
		}

		// Call keys callbacks
		for (auto& Iter : ChangedKeys)
		{
			auto& Callbacks = m_KeyModeToCallbacks.find({Iter.first, Iter.second});
			if (Callbacks != m_KeyModeToCallbacks.end())
			{
				for (InputCallbackHandler& CallbackHandler : Callbacks->second)
				{
					m_RegistedButtonCallbacks[CallbackHandler]();
				}
			}
		}

		// Call mouses callbacks
		for (auto& Iter : ChangedMouses)
		{
			auto& Callbacks = m_MouseModeToCallbacks.find({ Iter.first, Iter.second });
			if (Callbacks != m_MouseModeToCallbacks.end())
			{
				for (InputCallbackHandler& CallbackHandler : Callbacks->second)
				{
					m_RegistedButtonCallbacks[CallbackHandler]();
				}
			}
		}

		// Call mouse move callbacks
		glm::dvec2 PosDiff = CurrentCursorPos - m_LastCursorPos;
		if (!DOUBLE_EQUAL(glm::length(PosDiff), 0.f))
		{
			for (InputCallbackHandler& CallbackHandler : m_MouseMoveCallbacks)
			{
				m_RegistedMouseMoveCallbacks[CallbackHandler](m_LastCursorPos, CurrentCursorPos);
			}
			for (auto& Iter : m_MouseMoveCallbacksWithMouse)
			{
				if (m_LastMouseMode[Iter.first] == InputMode::KEY_PRESS)
				{
					for (InputCallbackHandler CallbackHandler : Iter.second)
					{
						m_RegistedMouseMoveCallbacks[CallbackHandler](m_LastCursorPos, CurrentCursorPos);
					}
				}
			}
			m_LastCursorPos = CurrentCursorPos;
		}
	}
}