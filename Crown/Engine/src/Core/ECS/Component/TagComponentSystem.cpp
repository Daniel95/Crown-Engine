#include "CrownPCH.h"
#include "TagComponentSystem.h"

#include "Core/ECS/Entity/Entity.h"

Crown::TagComponentSystem::TagComponentSystem()
{
}

void Crown::TagComponentSystem::OnUpdate(float a_DeltaTime)
{
}

void Crown::TagComponentSystem::OnAddEntity(Entity a_Entity)
{
}

void Crown::TagComponentSystem::OnRemoveEntity(Entity a_Entity)
{
}

void Crown::TagComponentSystem::SetTags(std::vector<std::pair<std::string, std::string>> a_Tags)
{
	m_Tags = a_Tags;
}

std::vector<std::pair<std::string, std::string>> Crown::TagComponentSystem::GetTags()
{
	return m_Tags;
}
