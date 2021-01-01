#pragma once

#include "Core/Event/Event.h"

namespace Crown
{
	//Params: float a_X, a_Y.
	class MouseMovedEvent : public Event<float, float> { };

	//Params: float a_XOffset, float a_YOffset.
	class MouseScrolledEvent : public Event<float, float> { };

	//Params: int a_Button.
	class MouseButtonEvent : public Event<int> { };

	//Params: int a_Button.
	class MouseButtonPressedEvent : public Event<int> { };

	//Params: int a_Button.
	class MouseButtonReleasedEvent : public Event<int> { };
}