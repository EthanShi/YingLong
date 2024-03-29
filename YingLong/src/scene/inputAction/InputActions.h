#pragma once

#include "core/Macros.h"
#include "core/Config.h"
#include "core/Log.h"
#include "input/Input.h" 

DECLARE_LOGGER(InputActionsLog)

#define INPUTACTION_TRIGGER_CALLBACK(ObjectPtr, FunctionName) [ObjectPtr]() { ObjectPtr->FunctionName(); }
#define INPUTACTION_AXIS_CALLBACK(ObjectPtr, FunctionName) [ObjectPtr](float Value) { ObjectPtr->FunctionName(Value); }

namespace YingLong {

	class Scene;

	class InputAction
	{
	public:
		using CallbackHandler = uint64;
		using TriggerActionCallback = std::function<void()>;
		using AxisActionCallback = std::function<void(float)>;

	private:
		union KeyOrMouseUnion
		{
			InputKey Key;
			InputMouse Mouse;

			friend bool operator==(KeyOrMouseUnion& A, KeyOrMouseUnion& B)
			{
				return A.Key == B.Key || A.Mouse == B.Mouse;
			}
		};

		struct TriggerActionBindInfo
		{
			TriggerActionBindInfo()
			{
				KeyOrMouse.Key = InputKey::KEY_UNKNOWN;
			}

			KeyOrMouseUnion KeyOrMouse;
			InputMode Mode;
		};

		struct AxisActionBindInfo
		{
			AxisActionBindInfo()
				: Scale(1.0f)
			{
				KeyOrMouse.Key = InputKey::KEY_UNKNOWN;
			}

			KeyOrMouseUnion KeyOrMouse;
			InputMode Mode;
			float Scale;
		};

		struct CallbackInfoBase
		{
			CallbackInfoBase(const CallbackHandler& Handler)
				: Handler(Handler)
			{}
			CallbackHandler Handler;
		};

		struct TriggerCallbackInfo : public CallbackInfoBase
		{
			TriggerCallbackInfo(const TriggerActionCallback& Callback, const CallbackHandler& Handler)
				: CallbackInfoBase(Handler)
				, Callback(std::move(Callback))
			{}
			TriggerActionCallback Callback;
		};

		struct AxisCallbackInfo : public CallbackInfoBase
		{
			AxisCallbackInfo(const AxisActionCallback& Callback, const CallbackHandler& Handler)
				: CallbackInfoBase(Handler)
				, Callback(std::move(Callback))
			{}
			AxisActionCallback Callback;
		};
		
		struct ActionInfoBase
		{
			std::string Name;
		};

		struct TriggerActionInfo : public ActionInfoBase
		{
			std::vector<TriggerActionBindInfo> BindInfo;
			std::vector <TriggerCallbackInfo> Callbacks;
		};

		struct AxisActionInfo : public ActionInfoBase
		{
			std::vector<AxisActionBindInfo> BindInfo;
			std::vector <AxisCallbackInfo> Callbacks;
		};

	public:
		using CallbackHandler = uint64;
		using TriggerActionCallback = std::function<void()>;
		using AxisActionCallback = std::function<void(float)>;

		InputAction();
		~InputAction();

		void Init(std::weak_ptr<Scene> Owner);

		CallbackHandler Bind(const std::string& ActionName, const TriggerActionCallback& Callback);
		CallbackHandler Bind(const std::string& ActionName, const AxisActionCallback& Callback);

		void UnBind(CallbackHandler Handler);

		void ClearCallbacks(const std::string& ActionName);
		void ClearCallbacks();

	private:
		void OnKeyChanged(InputKey Key, InputMode Mode);
		void OnMouseChanged(InputMouse Mouse, InputMode Mode);
		void OnMouseMove(const glm::vec2& OldPos, const glm::vec2& NewPos);
		void CallCallbacks(std::string ActionName, InputMode Mode, InputKey Key, InputMouse Mouse, float InputAxisValue = 1.0f);

		void ReadActions(const std::string& ActionGroupName);
		void AddAction(const std::string& ActionGroupName, const std::string& ActionName, const toml::array* InputsInfo);

	private:
		const std::string TrigerActionGroupName = "InputTriggerActions";
		const std::string AxisActionGroupName = "InputAxisActions";

		std::weak_ptr<Scene> OwnerScene;
		Input::CallbackHandler KeyEventHandler;
		Input::CallbackHandler MouseEventHandler;
		Input::CallbackHandler MouseMoveEventHandler;

		std::map<std::pair<InputKey, InputMode>, std::vector<std::string>> KeyToActionsMap;
		std::map<std::pair<InputMouse, InputMode>, std::vector<std::string>> MouseToActionsMap;

		CallbackHandler Handler = 0;

		std::unordered_map<CallbackHandler, std::string> HandlerToActionNameMap;
		std::unordered_map<std::string, TriggerActionInfo> TriggerActionsMap;
		std::unordered_map<std::string, AxisActionInfo> AxisActionsMap;
	};

}