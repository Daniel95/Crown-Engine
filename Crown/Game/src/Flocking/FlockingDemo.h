#pragma once
#include "Core/Application.h"
#include "Core/Utilities/Defines.h"

#ifdef CROWN_PC
#include <SFML/Graphics.hpp>
#endif

#include "Core/ECS/Entity/Entity.h"

class Neighborhood;
class Agent;

class FlockingDemo : public Crown::Application
{
public:
	FlockingDemo();
	~FlockingDemo();
protected:
	void OnUpdate(float a_DeltaTime) override;
	void OnRender() override;
	void OnStart() override;
	void OnLateUpdate(float a_DeltaTime) override;
	void OnFixedUpdate() override;
	void OnImGuiRender() override;
private:
	Crown::Entity m_NeighborhoodEntity;
	Crown::Entity m_PlayerControllerEntity;
	
#ifdef CROWN_PC
	sf::RenderWindow window;
	sf::CircleShape		m_DebugCircleShape;
	sf::RectangleShape	m_BinShape;
	sf::CircleShape		m_AgentBody;
#endif
};
