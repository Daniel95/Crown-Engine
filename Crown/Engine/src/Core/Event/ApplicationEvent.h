#pragma once

#include "Core/Event/Event.h"

namespace Crown 
{
	//Params: a_Width, a_Height.
	class WindowResizeEvent : public Event<int, int> { };

	class WindowCloseEvent : public Event<> { };

	//Params: a_DeltaTime.
	class UpdateEvent : public Event<float> { };

	class FixedUpdateEvent : public Event<> {};

	//Params: a_DeltaTime.
	class LateUpdateEvent : public Event<float> { };

	class ImGuiRenderEvent : public Event<> { };

	class RenderEvent : public Event<> { };
}
