#include "CrownPCH.h"
#include "EntityTracker.h"

namespace Crown
{
	int EntityTracker::s_Index = 0;

	int EntityTracker::Increment()
	{
		s_Index++;
		return s_Index;
	}
}

