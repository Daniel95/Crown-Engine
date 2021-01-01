#include <cstdlib>
#include <ctime>

#include "RoomBody.h"

RoomBody::RoomBody(RoomGeneration* a_Level)
{
	m_Width = 1;
	m_Height = 1;
	m_SpawnCooldown = 0.0f;
	m_RoomInfo = Group::start;
	m_Pos.XOffset = 0;
	m_Pos.YOffset = 0;
	m_Level = a_Level;
	m_DrawInfo.roomType = "ST";
	if (m_Level != nullptr)
	m_Level->m_Rooms.push_back(this);
	//How many neighboor rooms will I get as a starting room?
	int randomNumber = Roll(4) + 1;
	for(int i = 0; i < randomNumber; i++)
	{
		int Door = Roll(4);
		if(Door == 0)
		{
			m_Doors.push_back(Door::Up);
		}
		if (Door == 1)
		{
			m_Doors.push_back(Door::Right);
		}
		if (Door == 2)
		{
			m_Doors.push_back(Door::Down);
		}
		if (Door == 3)
		{
			m_Doors.push_back(Door::Left);
		}
	}
	if (DEBUGGING) {
		printf("First room");
		printf("\n");
	}
}

RoomBody::RoomBody(Door a_First, RoomGeneration* a_Level)
{
	m_SpawnCooldown = 0.0f;
	m_RoomInfo = GroupingSpawnDecider::WhichGroupSpawns();
	m_Doors.push_back(a_First);
	m_Width = 1;
	m_Height = 1;
	std::string first;
	if (a_First == Door::Up)
	{
		first = "U";
	}
	else if (a_First == Door::Right)
	{
		first = "R";
	}
	else if (a_First == Door::Down)
	{
		first = "D";
	}
	else if (a_First == Door::Left)
	{
		first = "L";
	}
	m_DrawInfo.roomType = first;
	m_Level = a_Level;
	if(m_Level != nullptr)
	m_Level->m_Rooms.push_back(this);
	if (DEBUGGING) {
		printf("Dead end spawned");
		printf("\n");
	}
}

RoomBody::RoomBody(Door a_First, Door a_Second, RoomGeneration* a_Level)
{
	m_SpawnCooldown = 0.0f;
	m_RoomInfo = GroupingSpawnDecider::WhichGroupSpawns();
	m_Doors.push_back(a_First);
	m_Doors.push_back(a_Second);
	m_Width = 1;
	m_Height = 1;
	std::string first;
	if (a_First == Door::Up)
	{
		first = "U";
	}
	else if (a_First == Door::Right)
	{
		first = "R";
	}
	else if (a_First == Door::Down)
	{
		first = "D";
	}
	else if (a_First == Door::Left)
	{
		first = "L";
	}
	std::string second;
	if (a_Second == Door::Up)
	{
		second = "U";
	}
	else if (a_Second == Door::Right)
	{
		second = "R";
	}
	else if (a_Second == Door::Down)
	{
		second = "D";
	}
	else if (a_Second == Door::Left)
	{
		second = "L";
	}
	m_DrawInfo.roomType = first + second;
	m_Level = a_Level;
	if (m_Level != nullptr)
	m_Level->m_Rooms.push_back(this);
	if (DEBUGGING) {
		printf("regular room");
		printf("\n");
	}
}

RoomBody::RoomBody(Door a_First, Door a_Second, Door a_Third, RoomGeneration* a_Level)
{
	m_SpawnCooldown = 0.0f;
	m_RoomInfo = GroupingSpawnDecider::WhichGroupSpawns();
	m_Doors.push_back(a_First);
	m_Doors.push_back(a_Second);
	m_Doors.push_back(a_Third);
	m_Width = 1;
	m_Height = 1;
	std::string first;
	if (a_First == Door::Up)
	{
		first = "U";
	}
	else if (a_First == Door::Right)
	{
		first = "R";
	}
	else if (a_First == Door::Down)
	{
		first = "D";
	}
	else if (a_First == Door::Left)
	{
		first = "L";
	}
	std::string second;
	if (a_Second == Door::Up)
	{
		second = "U";
	}
	else if (a_Second == Door::Right)
	{
		second = "R";
	}
	else if (a_Second == Door::Down)
	{
		second = "D";
	}
	else if (a_Second == Door::Left)
	{
		second = "L";
	}
	std::string third;
	if (a_Third == Door::Up)
	{
		third = "U";
	}
	else if (a_Third == Door::Right)
	{
		third = "R";
	}
	else if (a_Third == Door::Down)
	{
		third = "D";
	}
	else if (a_Third == Door::Left)
	{
		third = "L";
	}
	m_DrawInfo.roomType = first + second + third;
	m_Level = a_Level;
	if (m_Level != nullptr)
	m_Level->m_Rooms.push_back(this);
	if (DEBUGGING) {
		printf("this is not implemented yet lol");
	}
}


RoomBody::~RoomBody()
{
}

void RoomBody::Update(float a_DeltaTime)
{
	if (m_Doors.size() < 2)
	{
		if (m_Pos.XOffset == 0 && m_Pos.YOffset == 0)
		{
			std::printf("this should only be called once");
		}
		else { return; }
	}
	if (m_Level != nullptr) {
		if (m_SpawnCooldown <= 0.f)
		{
			//spawn
			for (Door door : m_Doors)
			{
				if (door == Door::Up)
				{
					//check available space
					if (m_Level->IsOffsetFree(m_Pos.XOffset, m_Pos.YOffset + 1))
					{
						//choose random room
						Door D{ Door::Down };
						Door DU[2]{ Door::Down, Door::Up };
						Door DR[2]{ Door::Down, Door::Right };
						Door DL[2]{ Door::Down, Door::Left };
						int random = std::rand() % 4;
						RoomBody* room;
						if (random == 0)
						{
							//Doors::Down
							room = new RoomBody(D, m_Level);
						}
						else if (random == 1)
						{
							if (m_Level->IsOffsetFree(m_Pos.XOffset, m_Pos.YOffset + 1 + 1))
							{
								room = new RoomBody(DU[0], DU[1], m_Level);
							}
							else
							{
								room = new RoomBody(D, m_Level);
							}
						}
						else if (random == 2)
						{
							if (m_Level->IsOffsetFree(m_Pos.XOffset + 1, m_Pos.YOffset + 1))
							{
								room = new RoomBody(DR[0], DR[1], m_Level);
							}
							else
							{
								room = new RoomBody(D, m_Level);
							}
						}
						else if (random == 3)
						{
							if (m_Level->IsOffsetFree(m_Pos.XOffset - 1, m_Pos.YOffset + 1))
							{
								room = new RoomBody(DL[0], DL[1], m_Level);
							}
							else
							{
								room = new RoomBody(D, m_Level);
							}
						}
						else
						{
							room = nullptr;
						}
						room->SetOffset(m_Pos.XOffset, m_Pos.YOffset + 1);
					}
				}
				if (door == Door::Right)
				{
					//check available space
					if (m_Level->IsOffsetFree(m_Pos.XOffset + 1, m_Pos.YOffset))
					{
						//choose random room
						Door L{ Door::Left };
						Door LU[2]{ Door::Left, Door::Up };
						Door LR[2]{ Door::Left, Door::Right };
						Door LD[2]{ Door::Left, Door::Down };
						int random = std::rand() % 4;
						RoomBody* room;
						if (random == 0)
						{
							//Doors::Left
							room = new RoomBody(L, m_Level);
						}
						else if (random == 1)
						{
							if (m_Level->IsOffsetFree(m_Pos.XOffset + 1, m_Pos.YOffset + 1))
							{
								room = new RoomBody(LU[0], LU[1], m_Level);
							}
							else
							{
								room = new RoomBody(L, m_Level);
							}
						}
						else if (random == 2)
						{
							if (m_Level->IsOffsetFree(m_Pos.XOffset + 1 + 1, m_Pos.YOffset))
							{
								room = new RoomBody(LR[0], LR[1], m_Level);
							}
							else
							{
								room = new RoomBody(L, m_Level);
							}
						}
						else if (random == 3)
						{
							if (m_Level->IsOffsetFree(m_Pos.XOffset + 1, m_Pos.YOffset - 1))
							{
								room = new RoomBody(LD[0], LD[1], m_Level);
							}
							else
							{
								room = new RoomBody(L, m_Level);
							}
						}
						else
						{
							room = nullptr;
						}
						room->SetOffset(m_Pos.XOffset + 1, m_Pos.YOffset);
					}
				}
				if (door == Door::Down)
				{
					//check available space
					if (m_Level->IsOffsetFree(m_Pos.XOffset, m_Pos.YOffset - 1))
					{
						//choose random room
						Door U{ Door::Up };
						Door UD[2]{ Door::Up, Door::Down };
						Door UR[2]{ Door::Up, Door::Right };
						Door UL[2]{ Door::Up, Door::Left };
						int random = std::rand() % 4;
						RoomBody* room;
						if (random == 0)
						{
							//Doors::Up
							room = new RoomBody(U, m_Level);
						}
						else if (random == 1)
						{
							if (m_Level->IsOffsetFree(m_Pos.XOffset, m_Pos.YOffset - 1 - 1))
							{
								room = new RoomBody(UD[0], UD[1], m_Level);
							}
							else
							{
								room = new RoomBody(U, m_Level);
							}
						}
						else if (random == 2)
						{
							if (m_Level->IsOffsetFree(m_Pos.XOffset + 1, m_Pos.YOffset - 1))
							{
								room = new RoomBody(UR[0], UR[1], m_Level);
							}
							else
							{
								room = new RoomBody(U, m_Level);
							}
						}
						else if (random == 3)
						{
							if (m_Level->IsOffsetFree(m_Pos.XOffset - 1, m_Pos.YOffset - 1))
							{
								room = new RoomBody(UL[0], UL[1], m_Level);
							}
							else
							{
								room = new RoomBody(U, m_Level);
							}
						}
						else
						{
							room = nullptr;
						}
						room->SetOffset(m_Pos.XOffset, m_Pos.YOffset - 1);
					}
				}
				if (door == Door::Left)
				{
					//check available space
					if (m_Level->IsOffsetFree(m_Pos.XOffset - 1, m_Pos.YOffset))
					{
						//choose random room
						Door R{ Door::Right };
						Door RU[2]{ Door::Right, Door::Up };
						Door RL[2]{ Door::Left, Door::Right };
						Door RD[2]{ Door::Right, Door::Down };
						int random = std::rand() % 4;
						RoomBody* room;
						if (random == 0)
						{
							//Doors::Right
							room = new RoomBody(R, m_Level);
						}
						else if (random == 1)
						{
							if (m_Level->IsOffsetFree(m_Pos.XOffset - 1, m_Pos.YOffset + 1))
							{
								room = new RoomBody(RU[0], RU[1], m_Level);
							}
							else
							{
								room = new RoomBody(R, m_Level);
							}
						}
						else if (random == 2)
						{
							if (m_Level->IsOffsetFree(m_Pos.XOffset - 1 - 1, m_Pos.YOffset))
							{
								room = new RoomBody(RL[0], RL[1], m_Level);
							}
							else
							{
								room = new RoomBody(R, m_Level);
							}
						}
						else if (random == 3)
						{
							if (m_Level->IsOffsetFree(m_Pos.XOffset - 1, m_Pos.YOffset - 1))
							{
								room = new RoomBody(RD[0], RD[1], m_Level);
							}
							else
							{
								room = new RoomBody(R, m_Level);
							}
						}
						else
						{
							room = nullptr;
						}
						room->SetOffset(m_Pos.XOffset - 1, m_Pos.YOffset);
					}
				}
			}
		}
		else
	{
	m_SpawnCooldown -= a_DeltaTime;
	}
	}
	
}

void RoomBody::SetOffset(int a_XOffset, int a_YOffset)
{
	m_Pos.XOffset = a_XOffset;
	m_Pos.YOffset = a_YOffset;
	if(DEBUGGING)
	printf("Offset: %i, %i\n", m_Pos.XOffset, m_Pos.YOffset);
}

DrawInfo RoomBody::Draw()
{
	m_DrawInfo.XOffset = m_Pos.XOffset;
	m_DrawInfo.YOffset = m_Pos.YOffset;
	return m_DrawInfo;
}

Position RoomBody::GetOffset()
{
	return m_Pos;
}
