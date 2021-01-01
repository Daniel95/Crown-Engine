#include "Flock.h"
#include "Proxy.h"
#include "Neighborhood.h"

#include "glm/trigonometric.hpp"
#include "glm/gtx/vector_angle.hpp"
//#include "glm/gtx/exterior_product.hpp"

using namespace Crown;

void Flock::OnUpdate(float a_DeltaTime)
{
	CROWN_PROFILE_FUNCTION();
	for (auto unit : GetEntities())
	{
		m_Data.at(unit).rotation = glm::degrees(glm::orientedAngle<float>(glm::normalize(glm::vec2(0.0f, 1.0f)), glm::normalize(m_Data.at(unit).currentVelocity)));
	}
}

void Flock::OnAddEntity(Entity a_Entity)
{
	ASSERT_ENGINE(m_Data.count(a_Entity) == 0, "Entity already exists!");	
	Flock::Data data = Flock::Data();
	data.proxy = new Proxy(a_Entity);
	std::pair<Entity, Data> pair{ a_Entity, data };
	m_Data.insert(pair);
}

void Flock::OnRemoveEntity(Entity a_Entity)
{
	ASSERT_ENGINE(m_Data.count(a_Entity) != 0, "Entity does not exist!");
	m_Data.at(a_Entity).proxy->m_MarkForDelete = true;
	m_Data.erase(a_Entity);
}

void Flock::Calibrate(Crown::Entity a_Entity, Crown::Entity a_Neighborhood)
{
	CROWN_PROFILE_FUNCTION();
	glm::vec2 topLeftBounds, bottomRightBounds;
	Application::Get().GetComponentSystemManager()->GetSystem<Neighborhood>()->GetGridBounds(a_Neighborhood, topLeftBounds, bottomRightBounds);
	Flock::Data data = m_Data.at(a_Entity);
	data.position = glm::vec2(randomFloat(topLeftBounds.x, bottomRightBounds.x), randomFloat(topLeftBounds.y, bottomRightBounds.y));
	data.direction = glm::normalize(glm::vec2(randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f)));
	data.rotation = glm::degrees(glm::orientedAngle<float>(glm::normalize(glm::vec2(0.0f, 1.0f)), glm::normalize(data.direction)));
	m_Data.at(a_Entity) = data;
}

void Flock::SetTarget(Crown::Entity a_Entity, glm::vec2 a_Target)
{
	CROWN_PROFILE_FUNCTION();
	Flock::Data& data = m_Data.at(a_Entity);
	data.target = a_Target;
}
