#pragma once

#include "Core/ECS/System/ComponentSystem.h"
#include "Core/ECS/Entity/Entity.h"

struct Proxy;
class Controller;
class Neighborhood;
class FlockingDemo;

class Flock : public Crown::ComponentSystem
{
	friend Controller;
	friend Neighborhood;
	friend FlockingDemo;
public:
	virtual void OnUpdate(float a_DeltaTime) override;
	virtual void OnAddEntity(Crown::Entity a_Entity) override;
	virtual void OnRemoveEntity(Crown::Entity a_Entity) override;

	void Calibrate(Crown::Entity a_Entity, Crown::Entity a_Neighborhood);

	Proxy* GetProxy(Crown::Entity a_Entity) { return m_Data.at(a_Entity).proxy; }
	glm::vec2 GetPosition(Crown::Entity a_Entity) { return m_Data.at(a_Entity).position; }
	void SetPosition(Crown::Entity a_Entity, glm::vec2 a_Position) { m_Data.at(a_Entity).position = a_Position; }

	glm::vec2 GetTarget(Crown::Entity a_Entity) const { return m_Data.at(a_Entity).target; }
	void SetTarget(Crown::Entity a_Entity, glm::vec2 a_Target);

	float GetPerceptionRadius(Crown::Entity a_Entity) const { return m_Data.at(a_Entity).perceptionRadius; }
	float GetGoalRadius(Crown::Entity a_Entity) const { return m_Data.at(a_Entity).goalRadiusSq; }
	float GetPrefSpeed(Crown::Entity a_Entity) const { return m_Data.at(a_Entity).prefSpeed; }
	float GetMaxAcceleration(Crown::Entity a_Entity) const { return m_Data.at(a_Entity).maxAccel; };

	void SetPrefVelocity(Crown::Entity a_Entity, glm::vec2 a_PrefVelocity) { m_Data.at(a_Entity).prefVelocity = a_PrefVelocity; }
	glm::vec2 GetFinalForce(Crown::Entity a_Entity) const { return m_Data.at(a_Entity).finalForce; }
	glm::vec2 GetCurrentVelocity(Crown::Entity a_Entity) const { return m_Data.at(a_Entity).currentVelocity; }
	void SetCurrentVelocity(Crown::Entity a_Entity, glm::vec2 a_CurrentVelocity) { m_Data.at(a_Entity).currentVelocity = a_CurrentVelocity; }
private:
	struct Data
	{
		bool isDebug = false;

		Proxy* proxy = nullptr;
		glm::vec2 position = glm::vec2();
		glm::vec2 direction = glm::vec2();
		float rotation = 0.0f;

		/// The radius of the character.
		float sizeRadius = 50.0f;

		// The goal of the character. 
		glm::vec2 target;
		// The maximum distance from the agent at which an object will be considered.
		float  perceptionRadius = 200.0f;
		/// The goal radius of the character
		float goalRadiusSq = 100.0f * 100.0f;
		/// The preferred speed of the character. 
		float prefSpeed = 50.0f;
		/// the preferred velocity of the character
		glm::vec2 prefVelocity;
		/// The velocity of the character
		glm::vec2 currentVelocity;
		/// The radius of the character.
		float _radius = 50.0f;
		/// The final force acting on the agent
		glm::vec2 finalForce;
		/// The maximum acceleration of the character.
		float maxAccel = 20.0f;

		/// The goal radius of the character
		float _goalRadiusSq = 50.0f;

		//additional parameters for the approach
		/// The scaling constant k of the anticipatory law
		float k = 1.5f;
		/// The exponential cutoff term tau_0
		float t0 = 3.0f;
		/// The exponent of the power law (m = 2 in our analysis)
		float m = 2.0f;
		/// Relaxation time for the driving force
		float ksi = 0.54f;
	};
	std::unordered_map<int, Data> m_Data;

};

inline float randomFloat(float a, float b)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}