
#include "YingLongPCH.h"

#include "Input.h"

namespace YingLong {

	void Input::InitInput(GLFWwindow* InWindow)
	{
		Window = InWindow;
		Handler = 0;
		RegistedButtonCallbacks.clear();
		RegistedMouseMoveCallbacks.clear();
		KeyModeToCallbacks.clear();
		MouseModeToCallbacks.clear();
		MouseMoveCallbacks.clear();
		MouseMoveCallbacksWithMouse.clear();
		LastKeyMode.clear();
		LastMouseMode.clear();
		LastCursorPos = { 0.f, 0.f };
	}

	void Input::SetCursorMode(CursorMode Mode)
	{
		glfwSetInputMode(Window, GLFW_CURSOR, (int)Mode);
	}

	CursorMode Input::GetCursorMode()
	{
		return (CursorMode)glfwGetInputMode(Window, GLFW_CURSOR);
	}

	void Input::AddCareKeys(std::vector<InputKey> Keys)
	{
		for (InputKey& Key : Keys)
		{
			auto FindResult = LastKeyMode.find(Key);
			if (FindResult == LastKeyMode.end())
			{
				LastKeyMode[Key] = (InputMode)glfwGetKey(Window, (int)Key);
			}
		}
	}

	void Input::AddCareMouses(std::vector<InputMouse> Mouses)
	{
		for (InputMouse& Mouse : Mouses)
		{
			auto FindResult = LastMouseMode.find(Mouse);
			if (FindResult == LastMouseMode.end())
			{
				LastMouseMode[Mouse] = (InputMode)glfwGetMouseButton(Window, (int)Mouse);
			}
		}
	}

	Input::CallbackHandler Input::BindKeyEvent(const KeyChangeCallback& Callback)
	{
		RegistedKeyChangeCallbacks[++Handler] = Callback;
		return Handler;
	}

	Input::CallbackHandler Input::BindKeyEvent(InputKey Key, InputMode Mode, const InputButtonCallback& Callback, bool CheckUnique)
	{
		std::pair<InputKey, InputMode> KeyTuple = { Key , Mode };
		if (KeyModeToCallbacks.find(KeyTuple) == KeyModeToCallbacks.end())
		{
			KeyModeToCallbacks.insert({ KeyTuple, std::vector<CallbackHandler>() });
		}
		auto& Callbacks = KeyModeToCallbacks[KeyTuple];

		// Check if Callback already registed for this Key-Mode
		if (CheckUnique)
		{
			for (const CallbackHandler& ExistCb : Callbacks)
			{
				if (Callback.target<void()>() == RegistedButtonCallbacks[ExistCb].target<void()>())
				{
					return ExistCb;
				}
			}
		}

		RegistedButtonCallbacks[++Handler] = Callback;
		Callbacks.push_back(Handler);

		AddCareKeys({ Key });

		return Handler;
	}

	Input::CallbackHandler Input::BindMouseEvent(const MouseChangeCallback& Callback)
	{
		RegistedMouseChangeCallbacks[++Handler] = Callback;
		return Handler;
	}

	Input::CallbackHandler Input::BindMouseEvent(InputMouse Mouse, InputMode Mode, const InputButtonCallback& Callback, bool CheckUnique)
	{
		std::pair<InputMouse, InputMode> KeyTuple = { Mouse , Mode };
		if (MouseModeToCallbacks.find(KeyTuple) == MouseModeToCallbacks.end())
		{
			MouseModeToCallbacks.insert({ KeyTuple, std::vector<CallbackHandler>() });
		}
		auto& Callbacks = MouseModeToCallbacks[KeyTuple];

		// Check if Callback already registed for this Key-Mode
		if (CheckUnique)
		{
			for (const CallbackHandler& ExistCb : Callbacks)
			{
				if (Callback.target<void()>() == RegistedButtonCallbacks[ExistCb].target<void()>())
				{
					return ExistCb;
				}
			}
		}

		RegistedButtonCallbacks[++Handler] = Callback;
		Callbacks.push_back(Handler);
		
		AddCareMouses({ Mouse });

		return Handler;
	}

	Input::CallbackHandler Input::BindMouseMoveEvent(const InputMouseMoveCallback& Callback, bool CheckUnique)
	{
		if (CheckUnique)
		{
			for (auto& ExistCb : RegistedMouseMoveCallbacks)
			{
				if (Callback.target<void()>() == ExistCb.second.target<void()>())
				{
					return ExistCb.first;
				}
			}
		}

		RegistedMouseMoveCallbacks[++Handler] = Callback;
		MouseMoveCallbacks.push_back(Handler);

		return Handler;
	}

	Input::CallbackHandler Input::BindMouseMoveEvent(InputMouse Mouse, const InputMouseMoveCallback& Callback, bool CheckUnique)
	{
		if (MouseMoveCallbacksWithMouse.find(Mouse) == MouseMoveCallbacksWithMouse.end())
		{
			MouseMoveCallbacksWithMouse.insert({ Mouse, std::vector<CallbackHandler>() });
		}

		if (CheckUnique)
		{
			for (auto& ExistCb : RegistedMouseMoveCallbacks)
			{
				if (Callback.target<void()>() == ExistCb.second.target<void()>())
				{
					return ExistCb.first;
				}
			}
		}

		RegistedMouseMoveCallbacks[++Handler] = Callback;
		MouseMoveCallbacksWithMouse[Mouse].push_back(Handler);

		return Handler;
	}

	void Input::UnBindInputEvent(const CallbackHandler& Handler)
	{
		RegistedButtonCallbacks.erase(Handler);
		RegistedMouseMoveCallbacks.erase(Handler);
		RegistedKeyChangeCallbacks.erase(Handler);
		RegistedMouseChangeCallbacks.erase(Handler);

		auto RemoveHandlerFromVector = [](std::vector<CallbackHandler>& Vector, const CallbackHandler& Handler)
		{
			auto WhereIter = std::remove(Vector.begin(), Vector.end(), Handler);
			if (WhereIter != Vector.end())
			{
				Vector.erase(WhereIter);
			}
		};

		for (auto& Iter : KeyModeToCallbacks)
		{
			RemoveHandlerFromVector(Iter.second, Handler);
		}
		for (auto& Iter : MouseModeToCallbacks)
		{
			RemoveHandlerFromVector(Iter.second, Handler);
		}

		RemoveHandlerFromVector(MouseMoveCallbacks, Handler);
		for (auto& Iter : MouseMoveCallbacksWithMouse)
		{
			RemoveHandlerFromVector(Iter.second, Handler);
		}
	}

	bool Input::IsKeyPressed(InputKey Key)
	{
		return glfwGetKey(Window, (int)Key) == GLFW_PRESS;
	}

	void Input::ProcessInput()
	{
		// Gather changed keys
		std::unordered_map<InputKey, InputMode> ChangedKeys;
		for (auto& Iter : LastKeyMode)
		{
			InputKey Key = Iter.first;
			InputMode Mode = Iter.second;
			InputMode CurrentKeyMode = (InputMode)glfwGetKey(Window, (int)Key);

			if (Mode == CurrentKeyMode)
			{
				continue;
			}

			ChangedKeys.insert({ Key, CurrentKeyMode });
			LastKeyMode[Key] = CurrentKeyMode;	// All modes in LastKeyMode updated
		}

		// Gather changed mouses
		std::unordered_map<InputMouse, InputMode> ChangedMouses;
		for (auto& Iter : LastMouseMode)
		{
			InputMouse Mouse = Iter.first;
			InputMode Mode = Iter.second;
			InputMode CurrentMouseMode = (InputMode)glfwGetKey(Window, (int)Mouse);

			if (Mode == CurrentMouseMode)
			{
				continue;
			}

			ChangedMouses.insert({ Mouse, CurrentMouseMode });
			LastMouseMode[Mouse] = CurrentMouseMode;
		}

		// Update mouse state for mouse move
		for (auto& Iter : MouseMoveCallbacksWithMouse)
		{
			LastMouseMode[Iter.first] = (InputMode)glfwGetKey(Window, (int)Iter.first);	// All modes in LastMouseMode updated
		}

		// Get current cursor position
		double CursorX, CursorY;
		glfwGetCursorPos(Window, &CursorX, &CursorY);
		glm::dvec2 CurrentCursorPos(CursorX, CursorY);
		if (DOUBLE_EQUAL(LastCursorPos.x, 0.f) && DOUBLE_EQUAL(LastCursorPos.y, 0.f))
		{
			LastCursorPos = CurrentCursorPos;
		}

		// Call keys callbacks
		for (auto& Iter : ChangedKeys)
		{
			auto Callbacks = KeyModeToCallbacks.find({Iter.first, Iter.second});
			if (Callbacks != KeyModeToCallbacks.end())
			{
				for (CallbackHandler& CallbackHandler : Callbacks->second)
				{
					RegistedButtonCallbacks[CallbackHandler]();
				}
			}
			for (auto& Callback : RegistedKeyChangeCallbacks)
			{
				Callback.second(Iter.first, Iter.second);
			}
		}

		// Call mouses callbacks
		for (auto& Iter : ChangedMouses)
		{
			auto Callbacks = MouseModeToCallbacks.find({ Iter.first, Iter.second });
			if (Callbacks != MouseModeToCallbacks.end())
			{
				for (CallbackHandler& CallbackHandler : Callbacks->second)
				{
					RegistedButtonCallbacks[CallbackHandler]();
				}
			}
			for (auto& Callback : RegistedMouseChangeCallbacks)
			{
				Callback.second(Iter.first, Iter.second);
			}
		}

		// Call mouse move callbacks
		glm::dvec2 PosDiff = CurrentCursorPos - LastCursorPos;
		if (!DOUBLE_EQUAL(glm::length(PosDiff), 0.f))
		{
			for (CallbackHandler& CallbackHandler : MouseMoveCallbacks)
			{
				RegistedMouseMoveCallbacks[CallbackHandler](LastCursorPos, CurrentCursorPos);
			}
			for (auto& Iter : MouseMoveCallbacksWithMouse)
			{
				if (LastMouseMode[Iter.first] == InputMode::KEY_PRESS)
				{
					for (CallbackHandler CallbackHandler : Iter.second)
					{
						RegistedMouseMoveCallbacks[CallbackHandler](LastCursorPos, CurrentCursorPos);
					}
				}
			}
			LastCursorPos = CurrentCursorPos;
		}
	}
}