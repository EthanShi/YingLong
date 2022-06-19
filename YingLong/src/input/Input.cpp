
#include "YingLongPCH.h"
#include "core/Macros.h"

#include "Input.h"

namespace YingLong {

	std::map<std::tuple<InputKey, InputKeyMode>, std::vector<InputCallback>> Input::m_RegistedKeyCallback;
	std::map<InputKey, InputKeyMode> Input::m_LastKeyMode;
	GLFWwindow* Input::m_Window = nullptr;

	void Input::InitInput(GLFWwindow* Window)
	{
		m_Window = Window;
	}

	bool Input::BindKeyEvent(InputKey Key, InputKeyMode Mode, const InputCallback& Callback)
	{
		std::tuple<InputKey, InputKeyMode> KeyTuple = { Key , Mode };
		if (m_RegistedKeyCallback.find(KeyTuple) == m_RegistedKeyCallback.end())
		{
			m_RegistedKeyCallback.insert({ KeyTuple, std::vector<InputCallback>() });
		}
		auto& Callbacks = m_RegistedKeyCallback.find(KeyTuple)->second;
		for (auto& ExistCb : Callbacks)
		{
			if (Callback.target<void()>() == ExistCb.target<void()>())
			{
				return false;
			}
		}
		Callbacks.push_back(Callback);

		const auto& LastKeyMode = m_LastKeyMode.find(Key);
		if (LastKeyMode == m_LastKeyMode.end())
		{
			m_LastKeyMode[Key] = Mode;
		}

		return true;
	}

	bool Input::UnBindKeyEvent(InputKey Key, InputKeyMode Mode, const InputCallback& Callback)
	{
		std::tuple<InputKey, InputKeyMode> KeyTuple = { Key , Mode };
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
			InputKeyMode Mode = std::get<1>(Elem.first);
			InputKeyMode CurrentKeyMode = (InputKeyMode)glfwGetKey(m_Window, (int)Key);

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
			m_LastKeyMode[Key] = (InputKeyMode)glfwGetKey(m_Window, (int)Key);
		}
	}

}