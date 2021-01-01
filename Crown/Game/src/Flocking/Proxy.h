#pragma once

#include <tuple>
#include <limits.h>
#include <glm/vec2.hpp>
#include "Core/ECS/Entity/Entity.h"
#include <iostream>

struct Proxy
{
	Proxy(Crown::Entity a_Parent)
		: m_Parent(a_Parent)
	{

	};

	~Proxy()
	{
		std::cout << "Proxy destructor" << std::endl;
	}

	bool operator==(const Proxy& other) const {
		return std::tie(this->m_BinIndex, this->m_Position, this->m_Parent) == std::tie(other.m_BinIndex, other.m_Position, other.m_Parent);
	}

	bool m_MarkForDelete = false;
	int m_BinIndex = INT_MIN;
	Crown::Entity m_Parent;
	glm::vec2 m_Position = glm::vec2(0.0f, 0.0f);
};