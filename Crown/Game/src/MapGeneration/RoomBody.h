#pragma once
#include <vector>
#include <string>
#include "RoomGeneration.h"
#include "GroupingSpawnDecider.h"

#define DEBUGGING false
enum class Door
{
	Up,
	Right,
	Down,
	Left
};

struct Position
{
	int XOffset, YOffset;
};

struct DrawInfo
{
	int XOffset, YOffset;
	std::string roomType;
};

class RoomBody
{
public:
	RoomBody(RoomGeneration* a_Level);
	RoomBody(Door a_First, RoomGeneration* a_Level);
	RoomBody(Door a_First, Door a_Second, RoomGeneration* a_Level);
	RoomBody(Door a_First, Door a_Second, Door a_Third, RoomGeneration* a_Level);
	~RoomBody();
	void Update(float a_DeltaTime);
	void SetOffset(int a_XOffset, int a_YOffset);
	DrawInfo Draw();
	Position GetOffset();


private:
	float m_SpawnCooldown;
	int m_Width;
	int m_Height;
	Position m_Pos;
	std::vector<Door> m_Doors;
	RoomGeneration* m_Level;
	DrawInfo m_DrawInfo;
	Group m_RoomInfo;
};
