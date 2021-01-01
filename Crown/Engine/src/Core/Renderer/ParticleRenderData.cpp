#include "CrownPCH.h"

#ifdef CROWN_PS4
#include "Platform/PS4/Renderer/PS4ParticleRenderData.h"
#endif
#ifdef CROWN_PC
#include "Platform/PC/Renderer/PCParticleRenderData.h"
#endif

namespace Crown
{
	ParticleRenderData* ParticleRenderData::Create()
	{
#ifdef CROWN_PS4
		return new PS4ParticleRenderData();
#endif

#ifdef CROWN_PC
		return new PCParticleRenderData();
#endif
	}
}