#include "Controller.h"
#include "Flock.h"
#include "Neighborhood.h"

#ifdef CROWN_PC
#include "sfml/Window/Joystick.hpp"
#endif

using namespace Crown;


void Controller::OnUpdate(float a_DeltaTime)
{
	CROWN_PROFILE_FUNCTION();
#ifdef CROWN_PC
	auto flock = Application::Get().GetComponentSystemManager()->GetSystem<Flock>();
	for (auto controller : GetEntities())
	{
		Controller::Data data = m_Data.at(controller);
		if (sf::Joystick::isConnected(0))
		{
			sf::Joystick::update();
			glm::vec2 input = glm::vec2(sf::Joystick::getAxisPosition(0, sf::Joystick::X), sf::Joystick::getAxisPosition(0, sf::Joystick::Y));
			input.x = -data.sensitivityTreshhold < input.x ? input.x < data.sensitivityTreshhold ? 0.0f : input.x : input.x;
			input.y = -data.sensitivityTreshhold < input.y ? input.y < data.sensitivityTreshhold ? 0.0f : input.y : input.y;

			if (glm::length(input) != 0.0f)
			{
				data.armyDirection = data.armyCenter + glm::normalize(input) * 250.0f;
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
		}
		m_Data.at(controller) = data;
	}
#endif
}

void Controller::OnAddEntity(Entity a_Entity)
{
	CROWN_PROFILE_FUNCTION();
	ASSERT_ENGINE(m_Data.count(a_Entity) == 0, "Entity already exists!");

	std::pair<Entity, Data> pair{ a_Entity, {} };
	m_Data.insert(pair);
}

void Controller::OnRemoveEntity(Entity a_Entity)
{
	CROWN_PROFILE_FUNCTION();
	ASSERT_ENGINE(m_Data.count(a_Entity) != 0, "Entity does not exist!");

	m_Data.erase(a_Entity);
}

void Controller::ResetController(Crown::Entity a_Controller, Crown::Entity a_Neighborhood)
{
	CROWN_PROFILE_FUNCTION();
	Controller::Data data = m_Data.at(a_Controller);
	for (auto unit : data.armyUnits)
	{
		unit.RemoveAllComponents();
	}
	m_Data.at(a_Controller) = data;
}

void Controller::AddToArmy(Crown::Entity a_Controller, Crown::Entity a_Neighborhood, uint a_Amount)
{
	CROWN_PROFILE_FUNCTION();
	Controller::Data data = m_Data.at(a_Controller);
	auto neighborhood = Application::Get().GetComponentSystemManager()->GetSystem<Neighborhood>();
	
	for (size_t i = 0; i < a_Amount; i++)
	{
		data.armyUnits.push_back(neighborhood->RegisterUnit(a_Neighborhood));
	}
	m_Data.at(a_Controller) = data;
}
