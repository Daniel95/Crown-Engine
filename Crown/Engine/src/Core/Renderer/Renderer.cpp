#include "CrownPCH.h"
#include "Renderer.h"

#ifdef CROWN_PS4
#include "Platform/PS4/Renderer/PS4Renderer.h"
#endif
#ifdef CROWN_PC
#include "Platform/PC/Renderer/PCRenderer.h"
#endif
namespace Crown
{
	std::shared_ptr<Renderer> Renderer::Create()
	{
	#ifdef CROWN_PS4
		return std::make_shared<PS4Renderer>();
	#endif

	#ifdef CROWN_PC
		return std::make_shared<PCRenderer>();
	#endif
	}

}