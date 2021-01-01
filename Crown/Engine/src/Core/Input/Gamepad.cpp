#include <CrownPCH.h>

#include "Gamepad.h"

#if defined(CROWN_PC)
#include "GLFW/glfw3.h"
#endif

namespace Crown {

	bool Gamepad::IsButtonDown(Button a_Button) const
	{
#if defined(CROWN_PS4)
		return (m_Buttons & static_cast<ButtonMask>(a_Button)) != 0;
#elif defined(CROWN_PC)
		return m_ButtonArray[static_cast<int>(a_Button)];
#endif
	}

	bool Gamepad::IsButtonPressed(Button a_ButtonId) const
	{
#if defined(CROWN_PS4)
		return (m_Buttons & static_cast<ButtonMask>(a_ButtonId)) != 0
			&& (m_PrevButtons & static_cast<ButtonMask>(a_ButtonId));
#elif defined(CROWN_PC)
		const auto id = static_cast<unsigned int>(a_ButtonId);
		return (m_ButtonArray[id] != 0) && (m_PrevButtonArray[id] == 0);
#endif
	}

	bool Gamepad::IsButtonReleased(Button a_Button) const
	{
#if defined(CROWN_PS4)
		return (m_Buttons & static_cast<ButtonMask>(a_Button)) != 0
			&& (m_PrevButtons & static_cast<ButtonMask>(a_Button));
#elif defined(CROWN_PC)
		const auto id = static_cast<unsigned int>(a_Button);
		return (m_ButtonArray[id] == 0) && (m_PrevButtonArray[id] != 0);
#endif
	}

	ButtonState Gamepad::GetButtonState(Button a_Button) const
	{
#if defined(CROWN_PS4)
		const auto isDown = (m_Buttons & static_cast<ButtonMask>(a_Button)) != 0;
		const auto wasDown = (m_PrevButtons & static_cast<ButtonMask>(a_Button)) != 0;
#elif defined(CROWN_PC)
		const auto isDown = m_ButtonArray[static_cast<int>(a_Button)];
		const auto wasDown = m_PrevButtonArray[static_cast<int>(a_Button)];
#endif

		if (isDown) {
			if (wasDown) return ButtonState::Down;
			return ButtonState::Pressed;
		}

		if (wasDown) return ButtonState::Released;
		return ButtonState::Up;
	}

#if defined(CROWN_PS4)
	bool Gamepad::CheckButtons(ButtonMask a_Buttons, ButtonSet a_Set) const
	{
		switch (a_Set) {
		case ButtonSet::None: return (m_Buttons & static_cast<ButtonMask>(a_Buttons)) == 0;
		case ButtonSet::Any: return (m_Buttons & static_cast<ButtonMask>(a_Buttons)) != 0;;
		case ButtonSet::All: return (m_Buttons & static_cast<ButtonMask>(a_Buttons)) == a_Buttons;;
		default: return false;
		}
	}
#endif

	float Gamepad::GetAxis(Axis a_Axis) const
	{
#if defined(CROWN_PS4)
		switch (a_Axis)
		{
		case Axis::LX: return m_LeftAnalogX;
		case Axis::LY: return m_LeftAnalogY;
		case Axis::RX: return m_RightAnalogX;
		case Axis::RY: return m_RightAnalogY;
		case Axis::TL: return m_LeftTrigger;
		case Axis::TR: return m_RightTrigger;
		default: return 0.0f;
		}
#elif defined(CROWN_PC)
		return m_AxisArray[static_cast<int>(a_Axis)];
#endif
	}

	float Gamepad::GetDir(Axis a_Axis) const
	{
		const auto value = GetAxis(a_Axis);
		return abs(value) >= m_LeftAnalogDeadZone ? Sign(value) : 0.0f;
	}

	Vec2 Gamepad::GetLeftAnalog() const
	{
		return { GetAxis(Axis::LX), GetAxis(Axis::LY) };
	}

	Vec2 Gamepad::GetRightAnalog() const
	{
		return { GetAxis(Axis::RX), GetAxis(Axis::RY) };
	}

	void Gamepad::SetLeftAnalogDeadZone(float a_Value)
	{
		m_LeftAnalogDeadZone = clamp(a_Value, 0.0f, 1.0f);
	}

	void Gamepad::SetRightAnalogDeadZone(float a_Value)
	{
		m_RightAnalogDeadZone = clamp(a_Value, 0.0f, 1.0f);

	}

	void Gamepad::SetTriggerDeadZone(float a_Value)
	{
		m_TriggerDeadZone = clamp(a_Value, 0.0f, 1.0f);
	}

	void Gamepad::Update()
	{
#if defined(CROWN_PS4)
		m_PrevButtons = m_Buttons;
#elif defined(CROWN_PC)
		GLFWgamepadstate state;
		if (glfwGetGamepadState(m_Id, &state))
		{
			std::copy(std::begin(m_ButtonArray), std::end(m_ButtonArray), m_PrevButtonArray.begin());
			std::copy(std::begin(state.buttons), std::end(state.buttons), m_ButtonArray.begin());
			std::copy(std::begin(state.axes), std::end(state.axes), m_AxisArray.begin());
		}
#endif
	}

}
