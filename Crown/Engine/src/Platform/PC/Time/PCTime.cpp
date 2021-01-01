#include "CrownPCH.h"

#include "Core/Time/Time.h"

#include "GLFW/glfw3.h"

namespace Crown
{
	float Time::GetTime()
	{
		CROWN_PROFILE_FUNCTION();
		
		return static_cast<float>(glfwGetTime());
	}
}
