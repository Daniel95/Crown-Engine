#include "Neighborhood.h"
#include <vector>
#include <iostream>
#include <iterator>
#include <glm/geometric.hpp>
#include "glm/trigonometric.hpp"
#include "glm/gtx/vector_angle.hpp"
//#include "glm/gtx/exterior_product.hpp"
#include "Bin.h"
#include "Flock.h"

using namespace Crown;

void Neighborhood::OnUpdate(float a_DeltaTime)
{
	CROWN_PROFILE_FUNCTION();
	auto flock = Application::Get().GetComponentSystemManager()->GetSystem<Flock>();
	for (auto neighborhood : GetEntities())
	{
		Neighborhood::Data data = m_Data.at(neighborhood);		
		auto itr = data.proxies.begin();
		while (itr != data.proxies.end())
		{
			auto proxy = (*itr);
			if (proxy->m_MarkForDelete)
			{
				m_Data.at(neighborhood).grid.at(static_cast<size_t>(proxy->m_BinIndex)).RemoveProxy(proxy);
				delete proxy;
				itr = data.proxies.erase(itr);
				continue;
			}

			DoFlockStep(neighborhood, proxy, a_DeltaTime);
			SphericalWrapAround(neighborhood, proxy);

			proxy->m_Position = flock->GetPosition(proxy->m_Parent);
			int currentID = CalcBinIdFromPosition(neighborhood, proxy->m_Position);
			if (proxy->m_BinIndex != currentID)
			{
				m_Data.at(neighborhood).grid.at(static_cast<size_t>(proxy->m_BinIndex)).RemoveProxy(proxy);
				proxy->m_BinIndex = currentID;
				m_Data.at(neighborhood).grid.at(static_cast<size_t>(proxy->m_BinIndex)).AddProxy(proxy);
			}

			itr++;
		}
		m_Data.at(neighborhood) = data;
	}
}

void Neighborhood::OnAddEntity(Entity a_Entity)
{
	ASSERT_ENGINE(m_Data.count(a_Entity) == 0, "Entity already exists!");

	std::pair<Entity, Data> pair{ a_Entity, {} };
	m_Data.insert(pair);
}

void Neighborhood::OnRemoveEntity(Entity a_Entity)
{
	ASSERT_ENGINE(m_Data.count(a_Entity) != 0, "Entity does not exist!");

	m_Data.erase(a_Entity);
}

int Neighborhood::CalcBinIdFromPosition(Entity a_Entity, glm::vec2 a_Position)
{
	CROWN_PROFILE_FUNCTION();
	glm::ivec2 index = glm::vec2();
	index.x = static_cast<int>((a_Position.x + m_Data.at(a_Entity).gridPositionOffset.x + m_Data.at(a_Entity).binSize.x / 2) / m_Data.at(a_Entity).binSize.x) % m_Data.at(a_Entity).collumCount;
	index.y = static_cast<int>((a_Position.y + m_Data.at(a_Entity).gridPositionOffset.y + m_Data.at(a_Entity).binSize.y / 2) / m_Data.at(a_Entity).binSize.y) % m_Data.at(a_Entity).rowCount;
	return (index.y * m_Data.at(a_Entity).collumCount + index.x);
}

void Neighborhood::DefineNeighborhood(Entity a_Entity, int a_Collums, int a_Rows, glm::vec2 a_BinSize)
{
	CROWN_PROFILE_FUNCTION();
	Neighborhood::Data data = m_Data.at(a_Entity);
	data.collumCount = a_Collums;
	data.rowCount = a_Rows;
	data.binSize = a_BinSize;
	data.gridPositionOffset = glm::vec2((0.5f * data.collumCount * data.binSize.x) - (0.5f * data.binSize.x), (0.5f * data.rowCount * data.binSize.y) - (0.5f * data.binSize.y));
	data.topLeftBounds = glm::vec2(-data.gridPositionOffset);
	data.bottomRightBounds = glm::vec2(data.gridPositionOffset);

	for (int y = 0; y < data.rowCount; y++)
	{
		for (int x = 0; x < data.collumCount; x++)
		{
			Bin bin = Bin(y * data.collumCount + x, glm::vec2(x * data.binSize.x - data.gridPositionOffset.x, y * data.binSize.y - data.gridPositionOffset.y), data.binSize);
			data.grid.push_back(bin);
		}
	}

	data.topLeftBounds = glm::vec2(-data.gridPositionOffset - (0.5f * data.binSize));
	data.bottomRightBounds = glm::vec2(data.gridPositionOffset + (0.5f * data.binSize));
	m_Data.at(a_Entity.GetId()) = data;
}

void Neighborhood::GetGridBounds(Entity a_Entity, glm::vec2& a_TopLeftBounds, glm::vec2& a_BottomRightBounds)
{
	CROWN_PROFILE_FUNCTION();
	a_TopLeftBounds = m_Data.at(a_Entity).topLeftBounds;
	a_BottomRightBounds = m_Data.at(a_Entity).bottomRightBounds;
}

void Neighborhood::DoFlockStep(Crown::Entity a_Entity, Proxy* a_Proxy, float a_DeltaTime)
{
	CROWN_PROFILE_FUNCTION();
	auto flock = Application::Get().GetComponentSystemManager()->GetSystem<Flock>();

	glm::vec2 prefVelocity = flock->GetTarget(a_Proxy->m_Parent) - a_Proxy->m_Position;
	float distSqToGoal = glm::length2(prefVelocity);

	if (distSqToGoal < flock->GetGoalRadius(a_Proxy->m_Parent))
	{
		return;
	}

	// compute preferred velocity
	prefVelocity *= flock->GetPrefSpeed(a_Proxy->m_Parent) / sqrtf(distSqToGoal);
	flock->SetPrefVelocity(a_Proxy->m_Parent, prefVelocity);

	// compute the new velocity of the agent
	ComputeForces(a_Entity, a_Proxy);

	// update the agents
	glm::vec2 acceleration = flock->GetFinalForce(a_Proxy->m_Parent);
	float maxAccel = flock->GetMaxAcceleration(a_Proxy->m_Parent);
	glm::vec2 currVelocity = flock->GetCurrentVelocity(a_Proxy->m_Parent);
	clamp(acceleration, maxAccel);
	currVelocity += acceleration * a_DeltaTime;
	glm::vec2 currPosition = flock->GetPosition(a_Proxy->m_Parent);
	currPosition += (currVelocity * a_DeltaTime);
	flock->SetCurrentVelocity(a_Proxy->m_Parent, currVelocity);
	flock->SetPosition(a_Proxy->m_Parent, currPosition);
}

void Neighborhood::ComputeForces(Crown::Entity a_Entity, Proxy* a_Proxy)
{
	CROWN_PROFILE_FUNCTION();
	auto flock = Application::Get().GetComponentSystemManager()->GetSystem<Flock>();
	Flock::Data& thisFlock = flock->m_Data.at(a_Proxy->m_Parent);

	//driving force
	thisFlock.finalForce = (thisFlock.prefVelocity - thisFlock.currentVelocity) / thisFlock.ksi;

	// compute the anticipatory force from each neighbor
	auto neighbors = GetNeighbors(a_Entity, a_Proxy->m_Position, thisFlock.perceptionRadius);
	for(Proxy* other : neighbors)
	{
		Flock::Data& otherFlock = flock->m_Data.at(other->m_Parent);
		
		const float distanceSq = glm::length2(other->m_Position - a_Proxy->m_Position);
		float radiusSq = pow(otherFlock.sizeRadius + thisFlock.sizeRadius, 2);
		if (a_Proxy != other && distanceSq != radiusSq)
		{
			// if agents are actually colliding use their separation distance 
			if (distanceSq < radiusSq)
				radiusSq = pow(otherFlock.sizeRadius + thisFlock.sizeRadius - sqrtf(distanceSq), 2);

			const glm::vec2 w = other->m_Position - a_Proxy->m_Position;
			const glm::vec2 v = thisFlock.currentVelocity - otherFlock.currentVelocity;
			const float a = glm::dot(v, v);
			const float b = glm::dot(w, v);
			const float c = glm::dot(w, w) - radiusSq;
			float discr = b * b - a * c;
			if (discr > .0f && (a<-FLT_EPSILON || a>FLT_EPSILON))
			{
				discr = sqrtf(discr);
				const float t = (b - discr) / a;
				if (t > 0) {
					thisFlock.finalForce += -thisFlock.k * exp(-t / thisFlock.t0) * (v - (b * v - a * w) / discr) / (a * powf(t, thisFlock.m)) * (thisFlock.m / t + 1 / thisFlock.t0);
				}
			}
		}

	}
	flock->m_Data.at(a_Proxy->m_Parent) = thisFlock;

}

void Neighborhood::SphericalWrapAround(Crown::Entity a_Entity, Proxy* a_Proxy)
{
	CROWN_PROFILE_FUNCTION();
	auto flock = Application::Get().GetComponentSystemManager()->GetSystem<Flock>();
	glm::vec2 position = flock->GetPosition(a_Proxy->m_Parent);

	Neighborhood::Data data = m_Data.at(a_Entity);
	if (position.x < data.topLeftBounds.x)
	{
		// left to right
		position.x = data.bottomRightBounds.x - FLT_EPSILON;
	}
	else if (data.bottomRightBounds.x < position.x)
	{
		// right to left
		position.x = data.topLeftBounds.x;// + 0.01f;
	}

	if (data.bottomRightBounds.y < position.y)
	{
		// bottom to top
		position.y = data.topLeftBounds.y;// +0.01f;
	}
	else if (position.y < data.topLeftBounds.y)
	{
		// top to bottom
		position.y = data.bottomRightBounds.y - FLT_EPSILON;
	}
	flock->SetPosition(a_Proxy->m_Parent, position);
}

std::vector<Proxy*> Neighborhood::GetNeighbors(Entity a_Entity, glm::vec2 a_Position, float a_SearchRadius)
{
	CROWN_PROFILE_FUNCTION();
	std::vector<Proxy*> neighbors = std::vector<Proxy*>();
	for (Bin bin : m_Data.at(a_Entity).grid)
	{
		if (bin.IsRelevant(a_Position, a_SearchRadius))
		{
			for (Proxy* proxy : bin.GetProxies())
			{
				neighbors.push_back(proxy);
			}
		}
	}
	return neighbors;
}

Crown::Entity Neighborhood::RegisterUnit(Crown::Entity a_Entity)
{
	CROWN_PROFILE_FUNCTION();
	auto unit = Crown::Entity::Create();
	auto flock = unit.AddToSystem<Flock>();
	flock->Calibrate(unit, a_Entity);
	auto proxy = flock->GetProxy(unit);
	m_Data.at(a_Entity).proxies.push_back(proxy);
	proxy->m_BinIndex = CalcBinIdFromPosition(a_Entity, proxy->m_Position);
	m_Data.at(a_Entity).grid.at(static_cast<size_t>(proxy->m_BinIndex)).AddProxy(proxy);
	return unit;
}
