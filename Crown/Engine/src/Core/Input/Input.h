#pragma once

#include "ActionMap.h"
#include "Gamepad.h"
#include "Keyboard.h"
#include "Mouse.h"

namespace Crown {
	
class Input
{
public:
	Gamepad& GetGamepad(int a_Jid);
	
	Keyboard& GetKeyboard();
	Mouse& GetMouse();

	ActionMap& GetActionMap();

	void Update();

private:
	std::unordered_map<int, Gamepad> m_Gamepads;
	Keyboard m_Keyboard;
	Mouse m_Mouse;

	ActionMap m_ActionMap{};
};
}