#include "MoveJob.h"
#include "Core/ECS/Component/ComponentManager.h"
#include "Core/ECS/Component/TransformComponentSystem.h"

#include "Core/Input/InputCommon.h"

void MoveJob::OnUpdate(int a_ThreadId)
{
	auto move = [](
		const std::vector<int>& a_Entities,
		std::unordered_map<int, Crown::TransformComponent>& a_TransformComponents,
		std::unordered_map<int, MoveComponent>& a_MoveComponents
		)
	{
		float deltaTime = Crown::Application::Get().GetDeltaTime();
		
		Crown::Input* input = Crown::Application::Get().GetInput();
		
		glm::vec2 horizontalInput = glm::vec3(0);
		
		if (input->GetKeyboard().IsKeyDown(GLFW_KEY_W))
		{
			horizontalInput.y = -1;
		}
		if (input->GetKeyboard().IsKeyDown(GLFW_KEY_S))
		{
			horizontalInput.y = 1;
		}
		if (input->GetKeyboard().IsKeyDown(GLFW_KEY_A))
		{
			horizontalInput.x = -1;
		}
		if (input->GetKeyboard().IsKeyDown(GLFW_KEY_D))
		{
			horizontalInput.x = 1;
		}

		if(horizontalInput != glm::vec2(0))
		{
			for (int entity : a_Entities)
			{
				auto transformIt = a_TransformComponents.find(entity);
				const auto moveIt = a_MoveComponents.find(entity);

				const glm::vec2 movement = horizontalInput * moveIt->second.speed * deltaTime;

				Crown::TransformHelper::TranslateLocal(transformIt->second, glm::vec3(movement.x, 0, movement.y));
			}
		}
	};

	Run<Crown::TransformComponent, MoveComponent>(move);
}
