#include "CrownPCH.h"
#include "ComponentSystemManager.h"

#include "Core/ECS/System/ComponentSystem.h"

namespace Crown
{
	void ComponentSystemManager::Update(float a_DeltaTime) const
	{
		CROWN_PROFILE_FUNCTION();

		for(const auto& system : m_Systems)
		{
			system->OnUpdate(a_DeltaTime);
		}
	}
}
