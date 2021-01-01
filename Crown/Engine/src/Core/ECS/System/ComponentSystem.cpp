#include "CrownPCH.h"
#include "ComponentSystem.h"
#include "Core/ECS/Entity/Entity.h"

namespace Crown
{
	void ComponentSystem::AddEntity(Entity a_Entity)
	{
		CROWN_PROFILE_FUNCTION();
		ASSERT_ENGINE(std::find(m_Entities.begin(), m_Entities.end(), a_Entity) == m_Entities.end(), "Entity is already added to this ComponentSystem!");

		m_Entities.push_back(a_Entity);
		OnAddEntity(a_Entity);
	}

	void ComponentSystem::RemoveEntity(Entity a_Entity)
	{
		CROWN_PROFILE_FUNCTION();
		ASSERT_ENGINE(std::find(m_Entities.begin(), m_Entities.end(), a_Entity) != m_Entities.end(), "Entity is not in this ComponentSystem!");
		
		OnRemoveEntity(a_Entity);
		m_Entities.erase(std::remove(m_Entities.begin(), m_Entities.end(), a_Entity), m_Entities.end());
	}

	void ComponentSystem::OnAddEntity(Entity a_Entity)
	{
	}

	void ComponentSystem::OnRemoveEntity(Entity a_Entity)
	{
	}
}
