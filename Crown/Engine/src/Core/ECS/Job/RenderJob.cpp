#include "CrownPCH.h"
#include "RenderJob.h"
#include "Core/ECS/Component/TransformComponentSystem.h"
#include "Core/ECS/Component/RenderComponentSystem.h"

namespace Crown
{
	void RenderJob::OnUpdate(int a_ThreadId)
	{
		auto func = [](
			const std::vector<int>& a_Entities,
			std::unordered_map<int, TransformComponent>& a_TransformComponents,
			std::unordered_map<int, RenderComponent>& a_RenderComponents
			)
		{
			for(int entity : a_Entities)
			{
				auto transformIt = a_TransformComponents.find(entity);
				auto renderIt = a_RenderComponents.find(entity);

				renderIt->second.modelMatrix = TransformHelper::GetWorldModelMatrix(transformIt->second);

				Application::Get().GetRenderer()->Submit(renderIt->second);
			}
		};

		Run<TransformComponent, RenderComponent>(func);
	}
}
