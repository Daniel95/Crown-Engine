#include "CrownPCH.h"
#include "Window.h"

#ifdef CROWN_PS4
#include "Platform/PS4/Renderer/PS4Window.h"
#endif
#ifdef CROWN_PC
#include "Platform/PC/PCWindow.h"
#endif

namespace Crown
{
	
	Window* Window::Create()
	{
#ifdef CROWN_PS4
		return new PS4Window();
#endif

#ifdef CROWN_PC
		return new PCWindow();
#endif
	}

}