#include "CrownPCH.h"
#include "Entity.h"
#include "EntityTracker.h"
#include "Core/Utilities/VectorHelper.h"

namespace Crown
{
	Entity::Entity(int a_Id)
		: m_Id(a_Id)
	{
	}

	void Entity::RemoveAllComponents() const
	{
		CROWN_PROFILE_FUNCTION();
		
		auto systems = Application::Get().GetComponentSystemManager()->GetAllSystems();

		for(const auto& system : systems)
		{
			if(VectorHelper::Contains(system->GetEntities(), *this))
			{
				system->RemoveEntity(*this);
			}
		}
	}

	Entity Entity::Create()
	{
		return Entity{ EntityTracker::Increment() };
	}
}
