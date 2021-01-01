#include "CrownPCH.h"
#include "Keyboard.h"

//TODO: Possible improvement
namespace Crown {

	void Keyboard::Update() {
		for (auto& key : m_KeyMap) {
			if (key.second == ButtonState::Pressed)
				key.second = ButtonState::Down;
			else if (key.second == ButtonState::Released)
				key.second = ButtonState::Up;
		}
	}

	void Keyboard::PressKey(unsigned int a_KeyId)
	{
		m_KeyMap[a_KeyId] = ButtonState::Pressed;
	}

	void Keyboard::ReleaseKey(unsigned int a_KeyId)
	{
		m_KeyMap[a_KeyId] = ButtonState::Released;
	}

	bool Keyboard::IsKeyDown(unsigned int a_KeyId) const
	{
		const auto state = GetState(a_KeyId);
		return state == ButtonState::Down || state == ButtonState::Pressed;
	}

	bool Keyboard::IsKeyPressed(unsigned int a_KeyId) const
	{
		return GetState(a_KeyId) == ButtonState::Pressed;
	}

	bool Keyboard::IsKeyReleased(unsigned int a_KeyId) const
	{
		return GetState(a_KeyId) == ButtonState::Released;
	}

	ButtonState Keyboard::GetState(unsigned int a_KeyId) const
	{
		const auto button = m_KeyMap.find(a_KeyId);
		if (button != m_KeyMap.end()) {
			return button->second;
		}
		return ButtonState::Up;
	}

}
