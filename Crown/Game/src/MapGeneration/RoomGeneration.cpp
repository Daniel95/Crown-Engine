#include "RoomGeneration.h"
#include <iostream>
#include "Mode.h"
#include "RoomBody.h"
#ifdef ROOMGENERATION
#include "EntryPoint.h"

Crown::Application* Crown::CreateApplication()
{
	return new RoomGeneration();
}
#endif


void RoomGeneration::OnUpdate(float a_DeltaTime)
{
	
}

void RoomGeneration::OnRender()
{
}

void RoomGeneration::OnStart()
{
	m_Rooms.push_back(new RoomBody(this));
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < static_cast<int>(m_Rooms.size()); j++)
		{
			m_Rooms[j]->Update(1.f);
		}
	}
	//get highest, lowest most right and most left point
	int highest = 0, lowest = 0, mostRight = 0, mostLeft = 0;
	for (int i = 0; i < static_cast<int>(m_Rooms.size()); i++)
	{
		if (m_Rooms[i]->Draw().XOffset < mostLeft)
		{
			mostLeft = m_Rooms[i]->Draw().XOffset;
		}
		if (m_Rooms[i]->Draw().XOffset > mostRight)
		{
			mostRight = m_Rooms[i]->Draw().XOffset;
		}
		if (m_Rooms[i]->Draw().YOffset < lowest)
		{
			lowest = m_Rooms[i]->Draw().YOffset;
		}
		if (m_Rooms[i]->Draw().YOffset > highest)
		{
			highest = m_Rooms[i]->Draw().YOffset;
		}
	}
	int difHeight = highest + std::abs(lowest) + 1;
	for (int i = 0; i < difHeight; i++)
	{
		std::string* str = new std::string();
		m_Map.push_back(str);
	}
	int difWidth = mostRight + std::abs(mostLeft) + 1;
	for (int y = 0; y < difHeight; y++)
	{
		for (int x = 0; x < difWidth; x++)
		{
			//Loop through each room to check if it has the 
			int XOffset = x + mostLeft;
			int YOffset = highest - y;
			bool gotRoom = false;
			const int roomSize = static_cast<int>(m_Rooms.size());

			for (int i = 0; i < roomSize; i++)
			{
				if (XOffset == m_Rooms[i]->GetOffset().XOffset && YOffset == m_Rooms[i]->GetOffset().YOffset)
				{
					if (m_Rooms[i]->Draw().roomType.size() == 1)
					{
						m_Map[y]->append(m_Rooms[i]->Draw().roomType + "_" + "\t");
					}
					else
					{
						m_Map[y]->append(m_Rooms[i]->Draw().roomType + "\t");
					}
					gotRoom = true;
				}
			}
			if (!gotRoom)
			{
				m_Map[y]->append("__\t");
			}
		}
	}

	if (DEBUGGING) {
		printf("\n");
		printf("If these values below differ, then cry please\n");
		printf("Map Width:\t%i\t%i\n", difWidth, (int)(m_Map[0]->size() / 3));
		printf("Map height:\t%i\t%i\n", difHeight, (int)m_Map.size());
		printf("Lowest X pos:\t%i\n", mostLeft);
		printf("Highest X pos:\t%i\n", mostRight);
		printf("Lowest Y pos:\t%i\n", lowest);
		printf("Highest Y pos:\t%i\n", highest);
		printf("\n");
	}
	for (int i = 0; i < m_Map.size(); i++)
	{
		std::cout << m_Map[i]->c_str() << std::endl;
	}
}

void RoomGeneration::OnLateUpdate(float a_DeltaTime)
{
}

void RoomGeneration::OnFixedUpdate()
{
}

void RoomGeneration::OnImGuiRender()
{
}

RoomGeneration::~RoomGeneration()
{
	for(int i = 0; i < m_Rooms.size(); i++)
	{
		delete m_Rooms[i];
	}
	m_Rooms.clear();
	m_Map.clear();
}

bool RoomGeneration::IsOffsetFree(int a_XOffset, int a_YOffset)
{
	for (int i = 0; i < static_cast<int>(m_Rooms.size()); i++)
	{
		if (m_Rooms[i]->GetOffset().XOffset == a_XOffset)
		{
			if (m_Rooms[i]->GetOffset().YOffset == a_YOffset)
			{
				return false;
			}
		}
	}
	return true;
}
