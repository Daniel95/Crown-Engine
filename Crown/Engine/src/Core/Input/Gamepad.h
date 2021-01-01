#pragma once

#include "Core/Input/InputCommon.h"
#include "Core/Utilities/Math.h"

#include <array>

namespace Crown {

	enum class ButtonState;

	class Gamepad
	{
	public:
		Gamepad() = default;

		bool IsButtonDown(Button a_Button) const;
		bool IsButtonPressed(Button a_ButtonId) const;
		bool IsButtonReleased(Button a_Button) const;
		ButtonState GetButtonState(Button a_Button) const;

#if defined(CROWN_PS4)
		bool CheckButtons(ButtonMask a_Buttons, ButtonSet a_Set) const;
#endif

		float GetAxis(Axis a_Axis) const;
		float GetDir(Axis a_Axis) const;

		Vec2 GetLeftAnalog() const;
		Vec2 GetRightAnalog() const;

		void SetLeftAnalogDeadZone(float a_Value);
		void SetRightAnalogDeadZone(float a_Value);
		void SetTriggerDeadZone(float a_Value);

		void Update();

	private:
		int m_Id;
		
#if defined(CROWN_PS4)
		float m_LeftAnalogX = 0.0f;
		float m_LeftAnalogY = 0.0f;
		float m_RightAnalogX = 0.0f;
		float m_RightAnalogY = 0.0f;
		float m_LeftTrigger = 0.0f;
		float m_RightTrigger = 0.0f;

		uint32_t m_PrevButtons = 0;
		uint32_t m_Buttons = 0;
#elif defined(CROWN_PC)
		std::array<float, AxisCount> m_AxisArray{};
		std::array<int, ButtonCount> m_PrevButtonArray{};
		std::array<int, ButtonCount> m_ButtonArray{};
#endif

		float m_LeftAnalogDeadZone = 0.1f;
		float m_RightAnalogDeadZone = 0.1f;
		float m_TriggerDeadZone = 0.5f;
	};
}
