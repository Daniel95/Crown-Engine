#include "FlockingDemo.h"

#include "Mode.h"
#include "Neighborhood.h"
#include "Controller.h"
#include "Flock.h"

#ifdef FLOCKINGDEMO
#include "EntryPoint.h"
Crown::Application* Crown::CreateApplication()
{
	Crown::Application* app = new FlockingDemo();
#ifdef CROWN_PC
	app->UseExternalRenderer(true);
#endif
	return app;
}
#endif

FlockingDemo::FlockingDemo()
	: m_NeighborhoodEntity(Crown::Entity::Create())
	, m_PlayerControllerEntity(Crown::Entity::Create())
{

}
FlockingDemo::~FlockingDemo()
{

}

void FlockingDemo::OnStart()
{
#ifdef CROWN_PC
	std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
	bool isFullscreen = false; //modes.size() > 0;
	window.create(isFullscreen ? modes.at(0) : sf::VideoMode(1920, 1080), "SFML Window", isFullscreen ? sf::Style::Fullscreen : sf::Style::Default);
	sf::View view = window.getView();
	view.setCenter(sf::Vector2f(0.0f, 0.0f));
	window.setView(view);
#endif  

	GetComponentSystemManager()->AddSystem<Neighborhood>();
	GetComponentSystemManager()->AddSystem<Controller>();
	GetComponentSystemManager()->AddSystem<Flock>();

	std::shared_ptr<Neighborhood> neighborhood = m_NeighborhoodEntity.AddToSystem<Neighborhood>();
	neighborhood->DefineNeighborhood(m_NeighborhoodEntity, 12, 7, { 150.0f, 150.0f });
	auto data = neighborhood->m_Data.at(m_NeighborhoodEntity);

	std::shared_ptr<Controller> controller = m_PlayerControllerEntity.AddToSystem<Controller>();
	controller->AddToArmy(m_PlayerControllerEntity, m_NeighborhoodEntity, 100);

#ifdef CROWN_PC
	float circleRadius = 10.0f;
	m_DebugCircleShape = sf::CircleShape(circleRadius, 14);
	m_DebugCircleShape.setOrigin(circleRadius, circleRadius);
	m_DebugCircleShape.setFillColor(sf::Color::Red);

	m_BinShape = sf::RectangleShape(sf::Vector2f(data.binSize.x, data.binSize.y));
	m_BinShape.setOrigin(sf::Vector2f(0.5f * data.binSize.x, 0.5f * data.binSize.y));
	m_BinShape.setFillColor(sf::Color::Transparent);
	m_BinShape.setOutlineThickness(1);
	m_BinShape.setOutlineColor(sf::Color::White);

	m_AgentBody = sf::CircleShape(50.0f, 3);
	m_AgentBody.setOrigin(sf::Vector2f(50.0f, 50.0f));
	m_AgentBody.setFillColor(sf::Color::Green);
	m_AgentBody.setScale(sf::Vector2f(0.5f, 1.0f));
#endif
}

void FlockingDemo::OnUpdate(float a_DeltaTime)
{
	GetComponentSystemManager()->GetSystem<Controller>()->OnUpdate(0.16f);
	GetComponentSystemManager()->GetSystem<Neighborhood>()->OnUpdate(0.16f);
	GetComponentSystemManager()->GetSystem<Flock>()->OnUpdate(0.16f);
}

void FlockingDemo::OnLateUpdate(float a_DeltaTime)
{
}

void FlockingDemo::OnFixedUpdate()
{
}

void FlockingDemo::OnRender()
{
#ifdef CROWN_PC
	window.clear();

	auto neighborhood = GetComponentSystemManager()->GetSystem<Neighborhood>();
	auto flock = GetComponentSystemManager()->GetSystem<Flock>();
	auto controller = GetComponentSystemManager()->GetSystem<Controller>();
	auto data = neighborhood->m_Data.at(m_NeighborhoodEntity);

	for (Bin bin : data.grid)
	{
		sf::Vector2f binPos = sf::Vector2f(bin.GetBinPosition().x, bin.GetBinPosition().y);
		m_BinShape.setPosition(binPos);
		window.draw(m_BinShape);
	}

	for (Crown::Entity ctrlr : controller->GetEntities())
	{
		 glm::vec2 center = controller->GetArmyCenter(ctrlr);
		 m_DebugCircleShape.setPosition(sf::Vector2f(center.x, center.y));
		 m_DebugCircleShape.setFillColor(sf::Color::Green);
		 window.draw(m_DebugCircleShape);

		 sf::VertexArray lineArray = sf::VertexArray();
		 lineArray.setPrimitiveType(sf::Lines);

		 for (auto unit : controller->GetArmyUnits(ctrlr))
		 {
			 glm::vec2 position = flock->GetPosition(unit);
			 lineArray.append(sf::Vertex(sf::Vector2f(position.x, position.y), sf::Color::Green));
			 lineArray.append(sf::Vertex(sf::Vector2f(center.x, center.y), sf::Color::Green));
			 glm::vec2 target = flock->GetTarget(unit);
			 lineArray.append(sf::Vertex(sf::Vector2f(position.x, position.y), sf::Color::Yellow));
			 lineArray.append(sf::Vertex(sf::Vector2f(target.x, target.y), sf::Color::Yellow));
		 }
		 window.draw(lineArray);

		 glm::vec2 goal = controller->GetArmyDirection(ctrlr);
		 m_DebugCircleShape.setFillColor(sf::Color::Yellow);
		 m_DebugCircleShape.setPosition(sf::Vector2f(goal.x, goal.y));
		 window.draw(m_DebugCircleShape);
		 m_DebugCircleShape.setFillColor(sf::Color::Red);

	}

	
	for (Crown::Entity unit : flock->GetEntities())
	{
		Flock::Data data = flock->m_Data.at(unit);
		m_AgentBody.setPosition(sf::Vector2f(data.position.x, data.position.y));
		m_AgentBody.setRotation(180.0f + data.rotation);
		window.draw(m_AgentBody);
	}	

	window.draw(m_DebugCircleShape);
	m_DebugCircleShape.setPosition(sf::Vector2f(data.topLeftBounds.x, data.topLeftBounds.y));
	window.draw(m_DebugCircleShape);
	m_DebugCircleShape.setPosition(sf::Vector2f(data.bottomRightBounds.x, data.bottomRightBounds.y));
	window.draw(m_DebugCircleShape);

	window.display();
#endif
}

void FlockingDemo::OnImGuiRender()
{
}