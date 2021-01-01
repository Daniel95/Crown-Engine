#pragma once

#include "Core/ECS/System/ComponentSystem.h"

namespace Crown
{
	class Entity;
	
	class TagComponentSystem : public ComponentSystem
	{
	public:
		TagComponentSystem();
		~TagComponentSystem() = default;

		void OnUpdate(float a_DeltaTime) override;
		void OnAddEntity(Entity a_Entity) override;
		void OnRemoveEntity(Entity a_Entity) override;
		void SetTags(std::vector<std::pair<std::string, std::string>> a_Tags);

		std::vector<std::pair<std::string, std::string>> GetTags();
	private:
		std::vector<std::pair<std::string, std::string>> m_Tags;
	};

}
