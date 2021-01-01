#include "CrownPCH.h"

#include "Input.h"

#if defined(CROWN_PC)
#include "GLFW/glfw3.h"
#endif

namespace Crown {

	Gamepad& Input::GetGamepad(int a_Jid)
	{
		// operator[ is ok since want an empty gamepad if it doesn't exist, but it would be valuable
		// provide some sort of error mechanism like a static empty gamepad. Ew, I know.
		return m_Gamepads[a_Jid];
	}

	Keyboard& Input::GetKeyboard()
	{
		return m_Keyboard;
	}

	Mouse& Input::GetMouse()
	{
		return m_Mouse;
	}

	ActionMap& Input::GetActionMap()
	{
		return m_ActionMap;
	}

	void Input::Update()
	{
#if defined(CROWN_PS4)
		// TODO: PS4 next block
#elif defined(CROWN_PC)
		
		m_Keyboard.Update();
		m_Mouse.Update();

		glfwPollEvents();

		for (auto j = 0; j < JoystickCount; ++j) {
			if (glfwJoystickPresent(j) == GLFW_TRUE) {
				auto cursor = m_Gamepads.find(j);
				if(cursor == m_Gamepads.end()) {
					const auto name = glfwGetJoystickName(j);
					std::cout << "Joystick " << name << " was connected: " << j << "\n";
					// TODO: first frame of input is missed, is this ok?
				} else {
					cursor->second.Update();
				}
			}
		}
#endif
	}
}
