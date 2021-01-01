#include "Bin.h"

Bin::Bin(int a_BinIndex, glm::vec2 a_Position, glm::vec2 a_Size)
	: m_BinIndex(a_BinIndex)
	, m_Position(a_Position)
	, m_Size(a_Size)
	, m_Proxies(std::list<Proxy*>())
{
}

bool Bin::IsRelevant(glm::vec2 a_Position, float a_SearchRadius)
{
	CROWN_PROFILE_FUNCTION();
	glm::vec2 topLeft(m_Position.x - 0.5f * m_Size.x, m_Position.y - 0.5f * m_Size.y);
	glm::vec2 bottomRight(m_Position.x + 0.5f * m_Size.x, m_Position.y + 0.5f * m_Size.y);
	float closestX = (a_Position.x < topLeft.x ? topLeft.x : (a_Position.x > bottomRight.x ? bottomRight.x : a_Position.x));
	float closestY = (a_Position.y < topLeft.y ? topLeft.y : (a_Position.y > bottomRight.y ? bottomRight.y : a_Position.y));
	float dx = closestX - a_Position.x;
	float dy = closestY - a_Position.y;
	return ((dx * dx + dy * dy) <= (a_SearchRadius * a_SearchRadius));
}

void Bin::AddProxy(Proxy* a_Proxy)
{
	CROWN_PROFILE_FUNCTION();
	m_Proxies.push_back(a_Proxy);
}

void Bin::RemoveProxy(Proxy* a_Proxy)
{
	CROWN_PROFILE_FUNCTION();
	m_Proxies.remove(a_Proxy);
}