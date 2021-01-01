#pragma once
#include "CORE/Utilities/Defines.h"

#include <unordered_map>
#include <memory>
#include "Core/ECS/Entity/Entity.h"

namespace Crown
{
	template<class T>
	class ComponentManager
	{
	public:
		static void Add(Entity a_Entity, T a_Component);
		static void Remove(Entity a_Entity);
		static std::unordered_map<int, T>& GetComponents() { return  s_Components; }
		static T& Get(Entity a_Entity);
		static void Set(Entity a_Entity, T& a_Component);
		
	private:
		static std::unordered_map<int, T> s_Components;
	};

	template<class T>
	std::unordered_map<int, T> ComponentManager<T>::s_Components;
	
	template <class T>
	void ComponentManager<T>::Add(Entity a_Entity, T a_Component)
	{
		ASSERT_ENGINE(s_Components.find(a_Entity) == s_Components.end(), "Component for this entity already exists!");
		
		std::pair<int, T> pair{ a_Entity, a_Component };
		s_Components.insert(pair);
	}

	template <class T>
	void ComponentManager<T>::Remove(Entity a_Entity)
	{
		ASSERT_ENGINE(s_Components.find(a_Entity) != s_Components.end(), "Component for this entity does not exist!");

		s_Components.erase(a_Entity);
	}

	template <class T>
	T& ComponentManager<T>::Get(Entity a_Entity)
	{
		ASSERT_ENGINE(s_Components.find(a_Entity) != s_Components.end(), "Component for this entity does not exist!");

		return s_Components[a_Entity];
	}

	template <class T>
	void ComponentManager<T>::Set(Entity a_Entity, T& a_Component)
	{
		ASSERT_ENGINE(s_Components.find(a_Entity) != s_Components.end(), "Component for this entity does not exist!");
		
		s_Components[a_Entity] = a_Component;
	}
}
