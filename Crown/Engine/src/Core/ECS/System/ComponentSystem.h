#pragma once

#include <vector>

namespace Crown
{
	class Entity;
	
	class ComponentSystem
	{
	public:
		ComponentSystem() = default;
		virtual ~ComponentSystem() = default;
		
		virtual void AddEntity(Entity a_Entity);
		void RemoveEntity(Entity a_Entity);
		virtual void OnUpdate(float a_DeltaTime) {};

		const std::vector<Entity>& GetEntities() const { return m_Entities; }
		
	protected:
		virtual void OnAddEntity(Entity a_Entity);
		virtual void OnRemoveEntity(Entity a_Entity);


	private:
		std::vector<Entity> m_Entities;

	};
}
