#pragma once

#include <unordered_map>

#include "InputCommon.h"

namespace Crown {

	class Keyboard
	{
	public:
		void Update();
		void PressKey(unsigned int a_KeyId);
		void ReleaseKey(unsigned int a_KeyId);

		bool IsKeyDown(unsigned int a_KeyId) const;
		bool IsKeyPressed(unsigned int a_KeyId) const;
		bool IsKeyReleased(unsigned int a_KeyId) const;
		ButtonState GetState(unsigned int a_KeyId) const;

	private:
		std::unordered_map<unsigned int, ButtonState> m_KeyMap {};
	};
}
