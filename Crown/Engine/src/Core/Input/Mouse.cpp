#include "CrownPCH.h"
#include "Mouse.h"

#include "ActionMap.h"
#include "Core/Application.h"

namespace Crown {

	void Mouse::Update()
	{
		std::copy(m_ButtonArray.begin(), m_ButtonArray.end(), m_PrevButtonArray.begin());
	}

	bool Mouse::IsButtonDown(MouseButton a_ButtonId) const
	{
		return m_ButtonArray[static_cast<int>(a_ButtonId)];
	}

	bool Mouse::WasButtonDown(MouseButton a_ButtonId) const
	{
		return m_PrevButtonArray[static_cast<int>(a_ButtonId)];
	}

	bool Mouse::IsButtonPressed(MouseButton a_ButtonId) const
	{
		return IsButtonDown(a_ButtonId) && !WasButtonDown(a_ButtonId);
	}

	bool Mouse::IsButtonReleased(MouseButton a_ButtonId) const
	{
		return !IsButtonDown(a_ButtonId) && WasButtonDown(a_ButtonId);
	}

	ButtonState Mouse::GetState(MouseButton a_Button) const
	{
		const auto isDown = IsButtonDown(a_Button);
		const auto wasDown = WasButtonDown(a_Button);
		
		if (isDown) {
			if (wasDown) return ButtonState::Down;
			return ButtonState::Pressed;
		}

		if (wasDown) return ButtonState::Released;
		return ButtonState::Up;
	}

	float Mouse::GetDuration(MouseButton a_Button) const
	{
		return m_ButtonArray[static_cast<unsigned int>(a_Button)] ? 1.0f : 0.0f;
	}

	void Mouse::PressButton(MouseButton a_ButtonId)
	{
		m_ButtonArray[static_cast<unsigned int>(a_ButtonId)] = true;
	}

	void Mouse::ReleaseButton(MouseButton a_ButtonId)
	{
		m_ButtonArray[static_cast<unsigned int>(a_ButtonId)] = false;
	}

	void Mouse::UpdateScroll(double xoffset, double yoffset)
	{
		m_PrevScroll = m_CurrScroll;
		m_CurrScroll += yoffset;
	}

	void Mouse::UpdateCoords(double a_X, double a_Y) {
		m_PrevMouseCoords = m_MouseCoords;
		m_MouseCoords = { float(a_X), float(a_Y) };
	}

	double Mouse::GetScroll() const
	{
		return m_CurrScroll - m_PrevScroll;
	}
	
	Vec2 Mouse::GetMotion() const
	{
		return m_MouseCoords - m_PrevMouseCoords;
	}
	
	Vec2 Mouse::GetCoords() const
	{
		return m_MouseCoords;
	}

}
