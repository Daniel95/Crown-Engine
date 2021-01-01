#include "CrowdJob.h"
#include "Core/ECS/Component/ComponentManager.h"
#include "Core/ECS/Component/TransformComponentSystem.h"
#include <thread>

void CrowdJob::OnUpdate(int a_ThreadId)
{
	auto move = [](
		const std::vector<int>& a_Entities,
		std::unordered_map<int, Crown::TransformComponent>& a_TransformComponent,
		std::unordered_map<int, CrowdComponent>& a_CrowdComponent
		)
	{
		float deltaTime = Crown::Application::Get().GetDeltaTime();
		
		for (int entity : a_Entities)
		{
			auto transformIt = a_TransformComponent.find(entity);
			const auto crowdComponent = a_CrowdComponent.find(entity);

			const glm::vec3 rotation = transformIt->second.localRotation + glm::vec3(0, 0, crowdComponent->second.rotateSpeed * deltaTime);

			Crown::TransformHelper::SetLocalRotation(transformIt->second, rotation);
		}
	};

	Run<Crown::TransformComponent, CrowdComponent>(move);
}
