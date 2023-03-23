
#include "YingLongPCH.h"
#include "InputTypes.h"

namespace YingLong {

	InputKey InputKeyFromString(const std::string& KeyName)
	{
		static std::unordered_map<std::string, InputKey> StringToKeyMap;
		if (StringToKeyMap.empty())
		{
			/* ,Printable keys = */
			StringToKeyMap["KEY_SPACE"] = InputKey::KEY_SPACE;
			StringToKeyMap["KEY_APOSTROPHE"] = InputKey::KEY_APOSTROPHE;/* ' */
			StringToKeyMap["KEY_COMMA"] = InputKey::KEY_COMMA;/* , */
			StringToKeyMap["KEY_MINUS"] = InputKey::KEY_MINUS;/* - */
			StringToKeyMap["KEY_PERIOD"] = InputKey::KEY_PERIOD;/* . */
			StringToKeyMap["KEY_SLASH"] = InputKey::KEY_SLASH;/* / */
			StringToKeyMap["KEY_0"] = InputKey::KEY_0;
			StringToKeyMap["KEY_1"] = InputKey::KEY_1;
			StringToKeyMap["KEY_2"] = InputKey::KEY_2;
			StringToKeyMap["KEY_3"] = InputKey::KEY_3;
			StringToKeyMap["KEY_4"] = InputKey::KEY_4;
			StringToKeyMap["KEY_5"] = InputKey::KEY_5;
			StringToKeyMap["KEY_6"] = InputKey::KEY_6;
			StringToKeyMap["KEY_7"] = InputKey::KEY_7;
			StringToKeyMap["KEY_8"] = InputKey::KEY_8;
			StringToKeyMap["KEY_9"] = InputKey::KEY_9;
			StringToKeyMap["KEY_SEMICOLON"] = InputKey::KEY_SEMICOLON;/* ; */
			StringToKeyMap["KEY_EQUAL"] = InputKey::KEY_EQUAL;/* = */
			StringToKeyMap["KEY_A"] = InputKey::KEY_A;
			StringToKeyMap["KEY_B"] = InputKey::KEY_B;
			StringToKeyMap["KEY_C"] = InputKey::KEY_C;
			StringToKeyMap["KEY_D"] = InputKey::KEY_D;
			StringToKeyMap["KEY_E"] = InputKey::KEY_E;
			StringToKeyMap["KEY_F"] = InputKey::KEY_F;
			StringToKeyMap["KEY_G"] = InputKey::KEY_G;
			StringToKeyMap["KEY_H"] = InputKey::KEY_H;
			StringToKeyMap["KEY_I"] = InputKey::KEY_I;
			StringToKeyMap["KEY_J"] = InputKey::KEY_J;
			StringToKeyMap["KEY_K"] = InputKey::KEY_K;
			StringToKeyMap["KEY_L"] = InputKey::KEY_L;
			StringToKeyMap["KEY_M"] = InputKey::KEY_M;
			StringToKeyMap["KEY_N"] = InputKey::KEY_N;
			StringToKeyMap["KEY_O"] = InputKey::KEY_O;
			StringToKeyMap["KEY_P"] = InputKey::KEY_P;
			StringToKeyMap["KEY_Q"] = InputKey::KEY_Q;
			StringToKeyMap["KEY_R"] = InputKey::KEY_R;
			StringToKeyMap["KEY_S"] = InputKey::KEY_S;
			StringToKeyMap["KEY_T"] = InputKey::KEY_T;
			StringToKeyMap["KEY_U"] = InputKey::KEY_U;
			StringToKeyMap["KEY_V"] = InputKey::KEY_V;
			StringToKeyMap["KEY_W"] = InputKey::KEY_W;
			StringToKeyMap["KEY_X"] = InputKey::KEY_X;
			StringToKeyMap["KEY_Y"] = InputKey::KEY_Y;
			StringToKeyMap["KEY_Z"] = InputKey::KEY_Z;
			StringToKeyMap["KEY_LEFT_BRACKET"] = InputKey::KEY_LEFT_BRACKET;/* [ */
			StringToKeyMap["KEY_BACKSLASH"] = InputKey::KEY_BACKSLASH;/* \ */
			StringToKeyMap["KEY_RIGHT_BRACKET"] = InputKey::KEY_RIGHT_BRACKET;/* ] */
			StringToKeyMap["KEY_GRAVE_ACCENT"] = InputKey::KEY_GRAVE_ACCENT;/* ` */
			StringToKeyMap["KEY_WORLD_1"] = InputKey::KEY_WORLD_1;/* non-US #1 */
			StringToKeyMap["KEY_WORLD_2"] = InputKey::KEY_WORLD_2;/* non-US #2 */

			/*,Function keys */
			StringToKeyMap["KEY_ESCAPE"] = InputKey::KEY_ESCAPE;
			StringToKeyMap["KEY_ENTER"] = InputKey::KEY_ENTER;
			StringToKeyMap["KEY_TAB"] = InputKey::KEY_TAB;
			StringToKeyMap["KEY_BACKSPACE"] = InputKey::KEY_BACKSPACE;
			StringToKeyMap["KEY_INSERT"] = InputKey::KEY_INSERT;
			StringToKeyMap["KEY_DELETE"] = InputKey::KEY_DELETE;
			StringToKeyMap["KEY_RIGHT"] = InputKey::KEY_RIGHT;
			StringToKeyMap["KEY_LEFT"] = InputKey::KEY_LEFT;
			StringToKeyMap["KEY_DOWN"] = InputKey::KEY_DOWN;
			StringToKeyMap["KEY_UP"] = InputKey::KEY_UP;
			StringToKeyMap["KEY_PAGE_UP"] = InputKey::KEY_PAGE_UP;
			StringToKeyMap["KEY_PAGE_DOWN"] = InputKey::KEY_PAGE_DOWN;
			StringToKeyMap["KEY_HOME"] = InputKey::KEY_HOME;
			StringToKeyMap["KEY_END"] = InputKey::KEY_END;
			StringToKeyMap["KEY_CAPS_LOCK"] = InputKey::KEY_CAPS_LOCK;
			StringToKeyMap["KEY_SCROLL_LOCK"] = InputKey::KEY_SCROLL_LOCK;
			StringToKeyMap["KEY_NUM_LOCK"] = InputKey::KEY_NUM_LOCK;
			StringToKeyMap["KEY_PRINT_SCREEN"] = InputKey::KEY_PRINT_SCREEN;
			StringToKeyMap["KEY_PAUSE"] = InputKey::KEY_PAUSE;
			StringToKeyMap["KEY_F1"] = InputKey::KEY_F1;
			StringToKeyMap["KEY_F2"] = InputKey::KEY_F2;
			StringToKeyMap["KEY_F3"] = InputKey::KEY_F3;
			StringToKeyMap["KEY_F4"] = InputKey::KEY_F4;
			StringToKeyMap["KEY_F5"] = InputKey::KEY_F5;
			StringToKeyMap["KEY_F6"] = InputKey::KEY_F6;
			StringToKeyMap["KEY_F7"] = InputKey::KEY_F7;
			StringToKeyMap["KEY_F8"] = InputKey::KEY_F8;
			StringToKeyMap["KEY_F9"] = InputKey::KEY_F9;
			StringToKeyMap["KEY_F10"] = InputKey::KEY_F10;
			StringToKeyMap["KEY_F11"] = InputKey::KEY_F11;
			StringToKeyMap["KEY_F12"] = InputKey::KEY_F12;
			StringToKeyMap["KEY_F13"] = InputKey::KEY_F13;
			StringToKeyMap["KEY_F14"] = InputKey::KEY_F14;
			StringToKeyMap["KEY_F15"] = InputKey::KEY_F15;
			StringToKeyMap["KEY_F16"] = InputKey::KEY_F16;
			StringToKeyMap["KEY_F17"] = InputKey::KEY_F17;
			StringToKeyMap["KEY_F18"] = InputKey::KEY_F18;
			StringToKeyMap["KEY_F19"] = InputKey::KEY_F19;
			StringToKeyMap["KEY_F20"] = InputKey::KEY_F20;
			StringToKeyMap["KEY_F21"] = InputKey::KEY_F21;
			StringToKeyMap["KEY_F22"] = InputKey::KEY_F22;
			StringToKeyMap["KEY_F23"] = InputKey::KEY_F23;
			StringToKeyMap["KEY_F24"] = InputKey::KEY_F24;
			StringToKeyMap["KEY_F25"] = InputKey::KEY_F25;
			StringToKeyMap["KEY_KP_0"] = InputKey::KEY_KP_0;
			StringToKeyMap["KEY_KP_1"] = InputKey::KEY_KP_1;
			StringToKeyMap["KEY_KP_2"] = InputKey::KEY_KP_2;
			StringToKeyMap["KEY_KP_3"] = InputKey::KEY_KP_3;
			StringToKeyMap["KEY_KP_4"] = InputKey::KEY_KP_4;
			StringToKeyMap["KEY_KP_5"] = InputKey::KEY_KP_5;
			StringToKeyMap["KEY_KP_6"] = InputKey::KEY_KP_6;
			StringToKeyMap["KEY_KP_7"] = InputKey::KEY_KP_7;
			StringToKeyMap["KEY_KP_8"] = InputKey::KEY_KP_8;
			StringToKeyMap["KEY_KP_9"] = InputKey::KEY_KP_9;
			StringToKeyMap["KEY_KP_DECIMAL"] = InputKey::KEY_KP_DECIMAL;
			StringToKeyMap["KEY_KP_DIVIDE"] = InputKey::KEY_KP_DIVIDE;
			StringToKeyMap["KEY_KP_MULTIPLY"] = InputKey::KEY_KP_MULTIPLY;
			StringToKeyMap["KEY_KP_SUBTRACT"] = InputKey::KEY_KP_SUBTRACT;
			StringToKeyMap["KEY_KP_ADD"] = InputKey::KEY_KP_ADD;
			StringToKeyMap["KEY_KP_ENTER"] = InputKey::KEY_KP_ENTER;
			StringToKeyMap["KEY_KP_EQUAL"] = InputKey::KEY_KP_EQUAL;
			StringToKeyMap["KEY_LEFT_SHIFT"] = InputKey::KEY_LEFT_SHIFT;
			StringToKeyMap["KEY_LEFT_CONTROL"] = InputKey::KEY_LEFT_CONTROL;
			StringToKeyMap["KEY_LEFT_ALT"] = InputKey::KEY_LEFT_ALT;
			StringToKeyMap["KEY_LEFT_SUPER"] = InputKey::KEY_LEFT_SUPER;
			StringToKeyMap["KEY_RIGHT_SHIFT"] = InputKey::KEY_RIGHT_SHIFT;
			StringToKeyMap["KEY_RIGHT_CONTROL"] = InputKey::KEY_RIGHT_CONTROL;
			StringToKeyMap["KEY_RIGHT_ALT"] = InputKey::KEY_RIGHT_ALT;
			StringToKeyMap["KEY_RIGHT_SUPER"] = InputKey::KEY_RIGHT_SUPER;
			StringToKeyMap["KEY_MENU"] = InputKey::KEY_MENU;
		};
		auto FindResult = StringToKeyMap.find(KeyName);
		if (FindResult == StringToKeyMap.end())
		{
			return InputKey::KEY_UNKNOWN;
		}
		return FindResult->second;
	}

	InputMode InputModeFromString(const std::string& InputModeName)
	{
		static std::unordered_map<std::string, InputMode> StringToInputModeMap;
		if (StringToInputModeMap.empty())
		{
			StringToInputModeMap["KEY_RELEASE"] = InputMode::KEY_RELEASE;
			StringToInputModeMap["KEY_PRESS"] = InputMode::KEY_PRESS;
			StringToInputModeMap["KEY_REPEAT"] = InputMode::KEY_REPEAT;
		}
		auto FindResult = StringToInputModeMap.find(InputModeName);
		if (FindResult == StringToInputModeMap.end())
		{
			return InputMode::INPUT_MODE_UNKNOWN;
		}
		return FindResult->second;
	}

	InputMouse InputMouseFromString(const std::string& InputMouseName)
	{
		static std::unordered_map<std::string, InputMouse> StringToInputMouseMap;
		if (StringToInputMouseMap.empty())
		{
			StringToInputMouseMap["MOUSE_BUTTON_1"] = InputMouse::MOUSE_BUTTON_1;
			StringToInputMouseMap["MOUSE_BUTTON_2"] = InputMouse::MOUSE_BUTTON_2;
			StringToInputMouseMap["MOUSE_BUTTON_3"] = InputMouse::MOUSE_BUTTON_3;
			StringToInputMouseMap["MOUSE_BUTTON_4"] = InputMouse::MOUSE_BUTTON_4;
			StringToInputMouseMap["MOUSE_BUTTON_5"] = InputMouse::MOUSE_BUTTON_5;
			StringToInputMouseMap["MOUSE_BUTTON_6"] = InputMouse::MOUSE_BUTTON_6;
			StringToInputMouseMap["MOUSE_BUTTON_7"] = InputMouse::MOUSE_BUTTON_7;
			StringToInputMouseMap["MOUSE_BUTTON_8"] = InputMouse::MOUSE_BUTTON_8;
			StringToInputMouseMap["MOUSE_BUTTON_LAST"] = InputMouse::MOUSE_BUTTON_LAST;
			StringToInputMouseMap["MOUSE_BUTTON_LEFT"] = InputMouse::MOUSE_BUTTON_LEFT;
			StringToInputMouseMap["MOUSE_BUTTON_RIGHT"] = InputMouse::MOUSE_BUTTON_RIGHT;
			StringToInputMouseMap["MOUSE_BUTTON_MIDDLE"] = InputMouse::MOUSE_BUTTON_MIDDLE;
			StringToInputMouseMap["MOUSE_X"] = InputMouse::MOUSE_X;
			StringToInputMouseMap["MOUSE_Y"] = InputMouse::MOUSE_Y;
		}
		auto FindResult = StringToInputMouseMap.find(InputMouseName);
		if (FindResult == StringToInputMouseMap.end())
		{
			return InputMouse::MOUSE_UNKNOWN;
		}
		return FindResult->second;
	}

	CursorMode CursorModeFromString(const std::string& CursorModeName)
	{
		static std::unordered_map<std::string, CursorMode> StringToCursorModeMap;
		if (StringToCursorModeMap.empty())
		{
			StringToCursorModeMap["CURSOR_NORMAL"] = CursorMode::CURSOR_NORMAL;
			StringToCursorModeMap["CURSOR_HIDDEN"] = CursorMode::CURSOR_HIDDEN;
			StringToCursorModeMap["CURSOR_DISABLED"] = CursorMode::CURSOR_DISABLED;
		}
		auto FindResult = StringToCursorModeMap.find(CursorModeName);
		if (FindResult == StringToCursorModeMap.end())
		{
			return CursorMode::CURSOR_MODE_UNKNOWN;
		}
		return FindResult->second;
	}

}