#include "AI_Controller.h"
#include "Flock.h"
#include "Neighborhood.h"
#ifdef CROWN_PC
#include "Core/Pathfinding/PathMovement.h"
#endif
using namespace Crown;


void AI_Controller::OnUpdate(float a_DeltaTime)
{
#ifdef CROWN_PC
	auto flock = Application::Get().GetComponentSystemManager()->GetSystem<Flock>();
	for (auto controller : GetEntities())
	{
		AI_Controller::Data data = m_Data.at(controller);

		if (controller.GetSystem<PathMovement>()->IsFlockingMovement(controller))
		{
			//since the pathfinding uses the 3d positions in the world, and the flocking only 2d I use glms inbuild function of defining a vec2 with a vec3
			data.armyDirection = data.armyCenter + glm::vec2(controller.GetSystem<PathMovement>()->GetDirection(controller)) * 250.0f;
		}
		else
		{
			data.armyDirection = data.armyCenter;
		}

		glm::vec2 centerPos = glm::vec2(0.0f, 0.0f);
		for each (Entity unit in data.armyUnits)
		{
			flock->SetTarget(unit, data.armyDirection);
			centerPos += flock->GetPosition(unit);
		}
		data.armyCenter = centerPos / static_cast<float>(data.armyUnits.size());

		m_Data.at(controller) = data;
	}
#endif
}

void AI_Controller::OnAddEntity(Entity a_Entity)
{
	ASSERT_ENGINE(m_Data.count(a_Entity) == 0, "Entity already exists!");

	std::pair<Entity, Data> pair{ a_Entity, {} };
	m_Data.insert(pair);
}

void AI_Controller::OnRemoveEntity(Entity a_Entity)
{
	ASSERT_ENGINE(m_Data.count(a_Entity) != 0, "Entity does not exist!");

	m_Data.erase(a_Entity);
}

void AI_Controller::ResetController(Crown::Entity a_Controller, Crown::Entity a_Neighborhood)
{
	AI_Controller::Data data = m_Data.at(a_Controller);
	for (auto unit : data.armyUnits)
	{
		unit.RemoveAllComponents();
	}
	m_Data.at(a_Controller) = data;
}

void AI_Controller::AddToArmy(Crown::Entity a_Controller, Crown::Entity a_Neighborhood, uint a_Amount)
{
	AI_Controller::Data data = m_Data.at(a_Controller);
	auto neighborhood = Application::Get().GetComponentSystemManager()->GetSystem<Neighborhood>();
	
	for (size_t i = 0; i < a_Amount; i++)
	{
		data.armyUnits.push_back(neighborhood->RegisterUnit(a_Neighborhood));
	}
	m_Data.at(a_Controller) = data;
}
