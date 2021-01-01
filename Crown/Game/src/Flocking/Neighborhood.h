#pragma once


#include "Core/ECS/System/ComponentSystem.h"
#include "Core/ECS/Entity/Entity.h"

#include "Bin.h"
#include "Proxy.h"

class FlockingDemo;

class Neighborhood : public Crown::ComponentSystem
{
	friend FlockingDemo;
public:
	Neighborhood() = default;
	~Neighborhood() = default;

	virtual void OnUpdate(float a_DeltaTime) override;
	virtual void OnAddEntity(Crown::Entity a_Entity) override;
	virtual void OnRemoveEntity(Crown::Entity a_Entity) override;

	void DefineNeighborhood(Crown::Entity a_Entity, int a_Collums, int a_Rows, glm::vec2 a_BinSize);
	void DefineNeighborhood(Crown::Entity a_Entity, glm::vec2 a_TopLeft, glm::vec2 a_BottomRight, glm::vec2 a_BinSize) {}; // TODO: Preffered method to automatically cover room depending on size.
	
	void GetGridBounds(Crown::Entity a_Entity, glm::vec2& a_TopLeftBounds, glm::vec2& a_BottomRightBounds);

	std::vector<Proxy*> GetNeighbors(Crown::Entity a_Entity, glm::vec2 a_Position, float a_SearchRadius);

	Crown::Entity RegisterUnit(Crown::Entity a_Entity);
private:
	struct Data
	{
		int					collumCount = 0;
		int					rowCount = 0;
		glm::vec2			binSize = glm::vec2(0.0f);
		glm::vec2			topLeftBounds = glm::vec2(0.0f);
		glm::vec2			bottomRightBounds = glm::vec2(0.0f);
		glm::vec2			gridPositionOffset = glm::vec2(0.0f);
		std::vector<Bin>	grid = std::vector<Bin>(0);
		std::vector<Proxy*>  proxies = std::vector<Proxy*>(0);
	};
	std::unordered_map<int, Data> m_Data;

	int CalcBinIdFromPosition(Crown::Entity a_Entity, glm::vec2 a_Position);
	void DoFlockStep(Crown::Entity a_Entity, Proxy* a_Proxy, float a_DeltaTime);
	void ComputeForces(Crown::Entity a_Entity, Proxy* a_Proxy);
	void SphericalWrapAround(Crown::Entity a_Entity, Proxy* a_Proxy);
};

/*!
	@brief Caps the magnitude of a vector to a maximum value.
	@param force A force vector
	@param maxValue The maximum magnitude of the force. */
inline void clamp(glm::vec2& v, float maxValue) {
	float lengthV = glm::length(v);
	if (lengthV > maxValue)
	{
		float mult = (maxValue / lengthV);
		v.x *= mult;
		v.y *= mult;
	}
}
