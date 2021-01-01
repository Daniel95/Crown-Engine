#include "CrownPCH.h"

#include "RenderComponentSystem.h"
#include "Core/ECS/Component/TransformComponentSystem.h"
#include "Core/ECS/Entity/Entity.h"
#include <utility>

namespace Crown
{
	RenderComponentSystem::RenderComponentSystem()
	{
		ASSERT_ENGINE(Application::Get().GetComponentSystemManager()->GetSystem<TransformComponentSystem>() != nullptr, "TransformComponentSystem ComponentSystem does not exists yet, create TransformComponentSystem ComponentSystem before creating the RenderComponentSystem ComponentSystem.");

		m_Transform = Application::Get().GetComponentSystemManager()->GetSystem<TransformComponentSystem>();
	}

	void RenderComponentSystem::OnUpdate(float a_DeltaTime)
	{
		if (Application::Get().UsingExternalRenderer()) { return; }

		for (auto data : m_Data)
		{
			data.second.modelMatrix = m_Transform->GetWorldModelMatrix(data.first);

			Application::Get().GetRenderer()->Submit(data.second);
		}
	}

	void RenderComponentSystem::OnAddEntity(Entity a_Entity)
	{
		ASSERT_ENGINE(m_Data.count(a_Entity) == 0, "Entity already exists!");

		std::pair<int, RenderComponent> pair{ a_Entity, {} };
		m_Data.insert(pair);
	}

	void RenderComponentSystem::OnRemoveEntity(Entity a_Entity)
	{
		ASSERT_ENGINE(m_Data.count(a_Entity) != 0, "Entity does not exist!");
		
		m_Data.erase(a_Entity);
	}

	void RenderComponentSystem::SetModel(Entity a_Entity, const std::shared_ptr<Model>& a_Model)
	{
		ASSERT_ENGINE(m_Data.count(a_Entity) != 0, "Entity does not exist!");

		m_Data.at(a_Entity).model = a_Model;
	}

	/*void RenderComponentSystem::SetMat(Entity a_Entity, const std::shared_ptr<Material>& a_Material)
	{
		ASSERT_ENGINE(m_Data.count(a_Entity) != 0, "Entity does not exist!");

		m_Data.at(a_Entity).material = a_Material;
	}*/
}
