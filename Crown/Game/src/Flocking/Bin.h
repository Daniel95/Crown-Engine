#pragma once

#include <limits.h>
#include <list>
#include <glm/vec2.hpp>
#include "Core/Utilities/Defines.h"
#include "Proxy.h"

class Bin
{
public:
	Bin() = default;
	Bin(int a_BinIndex, glm::vec2 a_Position, glm::vec2 a_Size);

	glm::vec2 GetBinPosition() const { return m_Position; };
	glm::vec2 GetBinSize() const { return m_Size; };
	bool IsRelevant(glm::vec2 a_Position, float a_SearchRadius);

	void AddProxy(Proxy* a_Proxy);
	void RemoveProxy(Proxy* a_Proxy);
	std::list<Proxy*> GetProxies() { return m_Proxies; };
private:
	int UNUSED(m_BinIndex) = INT_MIN;
	glm::vec2 m_Position = glm::vec2();
	glm::vec2 m_Size = glm::vec2();
	std::list<Proxy*> m_Proxies = std::list<Proxy*>();
};