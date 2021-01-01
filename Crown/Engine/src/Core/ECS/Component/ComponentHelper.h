#pragma once
#include "ComponentManager.h"

namespace Crown
{
	template<typename T1, typename T2>
	class ComponentPair
	{
	public:
		const T1 component1;
		const T2 component2;
	};

	class ComponentHelper
	{
	public:
		template<typename T1, typename T2>
		static std::unordered_map<int, ComponentPair<T1, T2>> GetComponentPairCollection();

		template<typename T1, typename T2>
		static std::vector<int> GetEntitiesWith();
		
	};

	template <typename T1, typename T2>
	std::unordered_map<int, ComponentPair<T1, T2>> ComponentHelper::GetComponentPairCollection()
	{
		std::vector<int> overlappingEntities = ComponentHelper::GetEntitiesWith<T1, T2>();

		const std::unordered_map<int, T1>& components1 = Crown::ComponentManager<T1>::GetComponents();
		const std::unordered_map<int, T2>& components2 = Crown::ComponentManager<T2>::GetComponents();
		
		std::unordered_map<int, ComponentPair<T1, T2>> collection;

		for (int entity : overlappingEntities)
		{
			ComponentPair<T1, T2> componentPair
			{
				components1.at(entity),
				components2.at(entity)
			};

			collection.insert({
				entity,
				componentPair
			});
		}

		return collection;
	}

	template <typename T1, typename T2>
	std::vector<int> ComponentHelper::GetEntitiesWith()
	{
		const std::unordered_map<int, T1>& components1 = ComponentManager<T1>::GetComponents();
		const std::unordered_map<int, T2>& components2 = ComponentManager<T2>::GetComponents();

		std::vector<int> overlappingEntities;

		for (auto it = components1.begin(); it != components1.end(); ++it)
		{
			if (components2.find(it->first) != components2.end())
			{
				overlappingEntities.push_back(it->first);
			}
		}

		return overlappingEntities;
	}
}
