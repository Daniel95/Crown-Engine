#pragma once

#include "Core/Application.h"
#include "Core/Utilities/Defines.h"
#include "Core/ECS/System/ComponentSystem.h"
#include "Core/ECS/System/ComponentSystemManager.h"

#include <vector>
#include <memory>

namespace Crown
{
	class Entity
	{
	public:
		Entity(int a_Id);
		~Entity() = default;
		//bool operator == (const Entity& a_OtherEntity) const { return m_Id == a_OtherEntity.GetId(); }
		//bool operator != (const Entity& a_OtherEntity) const { return !(this == &a_OtherEntity); }
		operator int() const { return m_Id; }

		template<typename T>
		std::shared_ptr<T> GetSystem() const;
		template<typename T>
		std::shared_ptr<T> AddToSystem() const;
		template<typename T>
		void RemoveFromSystem() const;
		void RemoveAllComponents() const;
		
		int GetId() const { return m_Id; }
		
		static Entity Create();
		
	private:
		 int m_Id = 0;
		
	};

	template<typename T>
	std::shared_ptr<T> Entity::AddToSystem() const
	{
		CROWN_PROFILE_FUNCTION();
		static_assert(std::is_base_of<ComponentSystem, T>::value, "Type must derive from Component");

		std::shared_ptr<T> componentSystem = Application::Get().GetComponentSystemManager()->GetSystem<T>();
		componentSystem->AddEntity(m_Id);

		return componentSystem;
	}

	template <typename T>
	void Entity::RemoveFromSystem() const
	{
		CROWN_PROFILE_FUNCTION();
		static_assert(std::is_base_of<ComponentSystem, T>::value, "Type must derive from Component");

		std::shared_ptr<ComponentSystem> componentSystem = Application::Get().GetComponentSystemManager()->GetSystem<T>();
		componentSystem->RemoveEntity(m_Id);
	}

	template <typename T>
	std::shared_ptr<T> Entity::GetSystem() const
	{
		CROWN_PROFILE_FUNCTION();
		static_assert(std::is_base_of<ComponentSystem, T>::value, "Type must derive from Component");

		std::shared_ptr<T> componentSystem = Application::Get().GetComponentSystemManager()->GetSystem<T>();
		return componentSystem;
	}
}