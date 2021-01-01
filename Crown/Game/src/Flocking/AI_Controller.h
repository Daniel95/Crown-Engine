#pragma once

#include "Core/ECS/System/ComponentSystem.h"
#include "Core/ECS/Entity/Entity.h"

class AI_Controller : public Crown::ComponentSystem
{
public:
	virtual void OnUpdate(float a_DeltaTime) override;
	virtual void OnAddEntity(Crown::Entity a_Entity) override;
	virtual void OnRemoveEntity(Crown::Entity a_Entity) override;

	void ResetController(Crown::Entity a_Controller, Crown::Entity a_Neighborhood);
	void AddToArmy(Crown::Entity a_Controller, Crown::Entity a_Neighborhood, uint a_Amount);

	std::vector<Crown::Entity> GetArmyUnits(Crown::Entity a_Controller) const { return m_Data.at(a_Controller).armyUnits; }
	glm::vec2 GetArmyCenter(Crown::Entity a_Controller) const { return m_Data.at(a_Controller).armyCenter; }
	glm::vec2 GetArmyDirection(Crown::Entity a_Controller) const { return m_Data.at(a_Controller).armyDirection; }
	
private:
	struct Data
	{
		//float sensitivityTreshhold = 7.5f;
		std::vector<Crown::Entity> armyUnits;
		glm::vec2 armyCenter = glm::vec2(0.0f);
		glm::vec2 armyDirection = glm::vec2(0.0f);
	};
	std::unordered_map<int, Data> m_Data;
};

