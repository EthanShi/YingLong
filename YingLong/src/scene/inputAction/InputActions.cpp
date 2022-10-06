
#include "YingLongPCH.h"

#include "InputActions.h"
#include "core/Config.h"
#include "scene/Scene.h"
#include "scene/events/EventTypes.h"

DEFINE_LOGGER(InputActionsLog)

namespace YingLong {

	InputAction::InputAction()
	{
		Input& InputInstance = Input::Instance();
		KeyEventHandler = InputInstance.BindKeyEvent([this](InputKey Key, InputMode Mode) { OnKeyChanged(Key, Mode); });
		MouseEventHandler = InputInstance.BindMouseEvent([this](InputMouse Mouse, InputMode Mode) { OnMouseChanged(Mouse, Mode); });
		MouseMoveEventHandler = InputInstance.BindMouseMoveEvent([this](const glm::dvec2& OldPos, const glm::dvec2& NewPos) { OnMouseMove(OldPos, NewPos); });
	}

	InputAction::~InputAction()
	{
		Input& InputInstance = Input::Instance();
		InputInstance.UnBindInputEvent(KeyEventHandler);
		InputInstance.UnBindInputEvent(MouseEventHandler);
		InputInstance.UnBindInputEvent(MouseMoveEventHandler);
		ClearCallbacks();
	}

	void InputAction::Init(std::weak_ptr<Scene> Owner)
	{
		m_OwnerScene = Owner;

		Input& InputInstance = Input::Instance();
		const toml::table& ConfigTable = Config::Instance().ReadOnly();

		ReadActions(m_TrigerActionGroupName);
		ReadActions(m_AxisActionGroupName);
	}

	InputAction::CallbackHandler InputAction::Bind(const std::string& ActionName, const TriggerActionCallback& Callback)
	{
		auto& TriggerAction = m_TriggerActionsMap.find(ActionName);
		if (TriggerAction == m_TriggerActionsMap.end())
		{
			return 0;
		}

		TriggerAction->second.Callbacks.push_back({ Callback, ++m_Handler });
		m_HandlerToActionNameMap[m_Handler] = ActionName;
		return m_Handler;
	}

	InputAction::CallbackHandler InputAction::Bind(const std::string& ActionName, const AxisActionCallback& Callback)
	{
		auto& TriggerAction = m_AxisActionsMap.find(ActionName);
		if (TriggerAction == m_AxisActionsMap.end())
		{
			return 0;
		}

		TriggerAction->second.Callbacks.push_back({ Callback, ++m_Handler });
		m_HandlerToActionNameMap[m_Handler] = ActionName;
		return m_Handler;
	}

	void InputAction::UnBind(CallbackHandler Handler)
	{
		auto& OwnerAction = m_HandlerToActionNameMap.find(Handler);
		if (OwnerAction == m_HandlerToActionNameMap.end())
		{
			return;
		}

		auto& TryTrigger = m_TriggerActionsMap.find(OwnerAction->second);
		if (TryTrigger != m_TriggerActionsMap.end())
		{
			auto& Callbacks = TryTrigger->second.Callbacks;
			Callbacks.erase(
				std::remove_if(Callbacks.begin(), Callbacks.end(),
					[Handler](const TriggerCallbackInfo& Info)
					{
						return Info.Handler == Handler;
					}),
				Callbacks.end());
		}

		auto& TryAxis = m_AxisActionsMap.find(OwnerAction->second);
		if (TryAxis != m_AxisActionsMap.end())
		{
			auto& Callbacks = TryAxis->second.Callbacks;
			Callbacks.erase(
				std::remove_if(Callbacks.begin(), Callbacks.end(),
					[Handler](const AxisCallbackInfo& Info)
					{
						return Info.Handler == Handler;
					}),
				Callbacks.end());
		}

		m_HandlerToActionNameMap.erase(Handler);
	}

	void InputAction::ClearCallbacks(const std::string& ActionName)
	{
		auto& TryTrigger = m_TriggerActionsMap.find(ActionName);
		if (TryTrigger != m_TriggerActionsMap.end())
		{
			for (auto& CallbackInfo : TryTrigger->second.Callbacks)
			{
				m_HandlerToActionNameMap.erase(CallbackInfo.Handler);
			}
			TryTrigger->second.Callbacks.clear();
		}

		auto& TryAxis = m_AxisActionsMap.find(ActionName);
		if (TryAxis != m_AxisActionsMap.end())
		{
			for (auto& CallbackInfo : TryAxis->second.Callbacks)
			{
				m_HandlerToActionNameMap.erase(CallbackInfo.Handler);
			}
			TryAxis->second.Callbacks.clear();
		}
	}

	void InputAction::ClearCallbacks()
	{
		m_HandlerToActionNameMap.clear();
		for (auto& Action : m_TriggerActionsMap)
		{
			Action.second.Callbacks.clear();
		}
		for (auto& Action : m_AxisActionsMap)
		{
			Action.second.Callbacks.clear();
		}
	}

	void InputAction::OnKeyChanged(InputKey Key, InputMode Mode)
	{
		auto& OwnerScene = m_OwnerScene.lock();
		auto& Actions = m_KeyToActionsMap.find({Key, Mode});
		if (!OwnerScene || Actions == m_KeyToActionsMap.end()) { return; }

		for (auto& ActionName : Actions->second)
		{
			CallCallbacks(ActionName, Mode, Key, InputMouse::MOUSE_UNKNOWN);
		}
	}

	void InputAction::OnMouseChanged(InputMouse Mouse, InputMode Mode)
	{
		auto& OwnerScene = m_OwnerScene.lock();
		auto& Actions = m_MouseToActionsMap.find({ Mouse, Mode });
		if (!OwnerScene || Actions == m_MouseToActionsMap.end()) { return; }

		for (auto& ActionName : Actions->second)
		{
			CallCallbacks(ActionName, Mode, InputKey::KEY_UNKNOWN, Mouse);
		}
	}

	void InputAction::OnMouseMove(const glm::vec2& OldPos, const glm::vec2& NewPos)
	{
		auto& OwnerScene = m_OwnerScene.lock();
		// Do MOUSE_X actions
		auto& Actions = m_MouseToActionsMap.find({ InputMouse::MOUSE_X, InputMode::KEY_PRESS });
		if (!OwnerScene || Actions == m_MouseToActionsMap.end()) { return; }

		for (auto& ActionName : Actions->second)
		{
			CallCallbacks(ActionName, InputMode::KEY_PRESS, InputKey::KEY_UNKNOWN, InputMouse::MOUSE_X, NewPos.x - OldPos.x);
		}

		// Do MOUSE_Y actions
		Actions = m_MouseToActionsMap.find({ InputMouse::MOUSE_Y, InputMode::KEY_PRESS });
		if (!OwnerScene || Actions == m_MouseToActionsMap.end()) { return; }

		for (auto& ActionName : Actions->second)
		{
			CallCallbacks(ActionName, InputMode::KEY_PRESS, InputKey::KEY_UNKNOWN, InputMouse::MOUSE_Y, NewPos.y - OldPos.y);
		}
	}

	void InputAction::ReadActions(const std::string& ActionGroupName)
	{
		Input& InputInstance = Input::Instance();
		const toml::table& ConfigTable = Config::Instance().ReadOnly();
		// Get ActionGroup witch contains a list of actions
		const toml::table* ActionGroup = ConfigTable.at(ActionGroupName).as_table();
		if (ActionGroup)
		{
			// Iter each Action { key: ActionName, value: InputsInfoList }
			ActionGroup->for_each([&ActionGroupName, this](const toml::key& Key, auto&& Val) {
				const toml::array* InputsInfo = Val.as_array();
				if (!InputsInfo)
				{
					InputActionsLog().error("ReadActions error, [{}] is not an array.", Key.str().data());
					return;
				}
				AddAction(ActionGroupName, Key.str().data(), InputsInfo);
				});
		}
		else
		{
			InputActionsLog().error("ReadActions error, [{}] is not an table.", ActionGroupName);
		}
	}

	void InputAction::AddAction(const std::string& ActionGroupName, const std::string& ActionName, const toml::array* InputsInfo)
	{
		/**
		* Iter each InputInfo :
		*	for TriggerActions: InputInfo only has "Key" bind with this action. 
		*	for AxisActions:	InputInfo has "Key" and "Scale".
		*/
		InputsInfo->for_each([&ActionGroupName, &ActionName, this](auto&& InputInfo) {
			const toml::table* InputInfoTable = InputInfo.as_table();
			if (!InputInfoTable)
			{
				InputActionsLog().error("ReadActions error, [{}] is not an table.", ActionName);
				return;
			}
			auto BindInputName = InputInfoTable->get_as<std::string>("InputName");
			if (!BindInputName)
			{
				return;
			}

			auto BindInputModeName = InputInfoTable->get_as<std::string>("InputMode");
			InputMode BindMode = InputModeFromString(BindInputModeName ? BindInputModeName->get() : "");
			if (BindMode == InputMode::INPUT_MODE_UNKNOWN)
			{
				BindMode = InputMode::KEY_PRESS;
			}

			std::vector<InputKey> CareKeys;
			std::vector<InputMouse> CareMouses;

			KeyOrMouseUnion KeyOrMouse { InputKey::KEY_UNKNOWN };

			InputKey TryKey = InputKeyFromString(BindInputName->get());
			if (TryKey != InputKey::KEY_UNKNOWN)
			{
				std::pair<InputKey, InputMode> FindKey(TryKey, BindMode);
				auto& FindResult = m_KeyToActionsMap.find(FindKey);
				if (FindResult == m_KeyToActionsMap.end())
				{
					m_KeyToActionsMap[FindKey] = {};
				}
				m_KeyToActionsMap[FindKey].push_back(ActionName);
				KeyOrMouse.Key = TryKey;
				CareKeys.push_back(TryKey);
			}

			InputMouse TryMouse = InputMouseFromString(BindInputName->get());
			if (TryMouse != InputMouse::MOUSE_UNKNOWN)
			{
				std::pair<InputMouse, InputMode> FindKey(TryMouse, BindMode);
				auto& FindResult = m_MouseToActionsMap.find(FindKey);
				if (FindResult == m_MouseToActionsMap.end())
				{
					m_MouseToActionsMap[FindKey] = {};
				}
				m_MouseToActionsMap[FindKey].push_back(ActionName);
				KeyOrMouse.Mouse = TryMouse;
				CareMouses.push_back(TryMouse);
			}

			Input& InputInstance = Input::Instance();
			InputInstance.AddCareKeys(CareKeys);
			InputInstance.AddCareMouses(CareMouses);

			if (ActionGroupName == m_TrigerActionGroupName)
			{
				TriggerActionBindInfo BindInfo;
				BindInfo.KeyOrMouse = KeyOrMouse;
				BindInfo.Mode = BindMode;
				auto& ActionInfoFindResult = m_TriggerActionsMap.find(ActionName);
				if (ActionInfoFindResult == m_TriggerActionsMap.end())
				{
					TriggerActionInfo NewActionInfo;
					NewActionInfo.Name = ActionName;
					m_TriggerActionsMap[ActionName] = std::move(NewActionInfo);
				}
				auto& ActionInfo = m_TriggerActionsMap[ActionName];
				ActionInfo.BindInfo.push_back(BindInfo);
			}
			else if (ActionGroupName == m_AxisActionGroupName)
			{
				AxisActionBindInfo BindInfo;
				BindInfo.KeyOrMouse = KeyOrMouse;
				auto Scale = InputInfoTable->get_as<double>("Scale");
				BindInfo.Mode = BindMode;
				if (Scale)
				{
					BindInfo.Scale = float(double(*Scale));
				}
				auto& ActionInfoFindResult = m_AxisActionsMap.find(ActionName);
				if (ActionInfoFindResult == m_AxisActionsMap.end())
				{
					AxisActionInfo NewActionInfo;
					NewActionInfo.Name = ActionName;
					m_AxisActionsMap[ActionName] = std::move(NewActionInfo);
				}
				auto& ActionInfo = m_AxisActionsMap[ActionName];
				ActionInfo.BindInfo.push_back(BindInfo);
			}
		});
	}

	void InputAction::CallCallbacks(std::string ActionName, InputMode Mode, InputKey Key, InputMouse Mouse, float InputAxisValue)
	{
		auto& TriggerAction = m_TriggerActionsMap.find(ActionName);
		if (TriggerAction != m_TriggerActionsMap.end())
		{
			for (auto& Callback : TriggerAction->second.Callbacks)
			{
				Callback.Callback();
			}
		}

		KeyOrMouseUnion KeyOrMouse;
		if (Key != InputKey::KEY_UNKNOWN)
		{
			KeyOrMouse.Key = Key;
		}
		else
		{
			KeyOrMouse.Mouse = Mouse;
		}

		auto& AxisAction = m_AxisActionsMap.find(ActionName);
		if (AxisAction != m_AxisActionsMap.end())
		{
			float Scale = 1.0f;
			for (auto& BindInfo : AxisAction->second.BindInfo)
			{
				if (BindInfo.KeyOrMouse == KeyOrMouse && BindInfo.Mode == Mode)
				{
					Scale = BindInfo.Scale * InputAxisValue;
					break;
				}
			}
			for (auto& Callback : AxisAction->second.Callbacks)
			{
				Callback.Callback(Scale);
			}
		}
	}
}