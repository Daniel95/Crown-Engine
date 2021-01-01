#pragma once

#include "Core/Utilities/Math.h"
#include "InputCommon.h"

#include <array>

namespace Crown {

	class Mouse
	{
	public:
		void Update();

		bool IsButtonDown(MouseButton a_ButtonId) const;
		bool IsButtonPressed(MouseButton a_ButtonId) const;
		bool IsButtonReleased(MouseButton a_ButtonId) const;
		bool WasButtonDown(MouseButton a_ButtonId) const;
		
		ButtonState GetState(MouseButton a_Button) const;
		float GetDuration(MouseButton a_Button) const;

		void PressButton(MouseButton a_ButtonId);
		void ReleaseButton(MouseButton a_ButtonId);

		void UpdateScroll(double a_XOffset, double a_YOffset);

		double GetScroll() const;
		Vec2 GetMotion() const;;
		Vec2 GetCoords() const;

		void UpdateCoords(double a_X, double a_Y);

	private:
		std::array<bool, MouseButtonCount> m_ButtonArray {};
		std::array<bool, MouseButtonCount> m_PrevButtonArray {};
		
		Vec2 m_MouseCoords {};
		Vec2 m_PrevMouseCoords {};
		double m_CurrScroll {};
		double m_PrevScroll {};
	};

}
