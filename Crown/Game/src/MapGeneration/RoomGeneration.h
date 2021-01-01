#pragma once

#include "Core/Application.h"
#include <vector>
#include <chrono>
#if defined(CROWN_PC)
#include <SFML/Graphics/RenderWindow.hpp>
#endif


class RoomBody;

namespace Crown
{
	class RenderComponentSystem;
	class TransformComponentSystem;
}
//Returns random int below max (max not included)
//Counts on that there is a random seed provided in initialization
//MUST BE AFTER SEED INITIALIZATION
static int Roll(int max)
{
	const int random = std::rand() % max;
	return random;
}
class RoomGeneration : public Crown::Application
{
protected:
	void OnUpdate(float a_DeltaTime) override;
	void OnRender() override;
	void OnStart() override;
	void OnLateUpdate(float a_DeltaTime) override;
	void OnFixedUpdate() override;
	void OnImGuiRender() override;
	~RoomGeneration();
public:
	std::vector<RoomBody*> GetRooms();
	bool IsOffsetFree(int a_XOffset, int a_YOffset);
	std::vector<RoomBody*> m_Rooms;
private:
	//sf::RenderWindow m_Window;
	std::vector<std::string*> m_Map;
};


