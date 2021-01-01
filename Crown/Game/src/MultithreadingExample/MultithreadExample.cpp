#include "MultithreadExample.h"
#include <iostream>

#include <Core/Utilities/Log.h>
#include "Mode.h"
#include "Core/ECS/Entity/Entity.h"
#include "Core/ECS/Component/TransformComponentSystem.h"
#include "Core/ECS/Component/RenderComponentSystem.h"
#include "Core/Utilities/VectorHelper.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Renderer/GLTFLoading/GLTFParser.h"
#include "Core/Renderer/Models.h"
#include "Core/ECS/Component/CameraComponentSystem.h"
#include "Core/Utilities/Defines.h"
#include "Core/ECS/Component/ComponentManager.h"
#include "Core/ECS/Component/CameraComponentSystem.h"
#include "Core/ECS/Component/TransformComponentSystem.h"
#include "Core/ECS/Component/ComponentHelper.h"
#include <thread>
#include "Core/ECS/Job/JobManager.h"
#include "Core/ECS/Job/RenderJob.h"
#include "ECS/MoveJob.h"
#include "ECS/CrowdJob.h"
#include "Core/ECS/Component/CameraHelper.h"
#include "ECS/MoveCameraJob.h"

#ifdef MULTITHREADEXAMPLE
#include "EntryPoint.h"
Crown::Application* Crown::CreateApplication()
{
    return new MultithreadExample();
}
#endif

void MultithreadExample::OnUpdate(float a_DeltaTime)
{
	/*
    {
        std::vector<int> entitiesWithComponents = Crown::ComponentHelper::GetEntitiesWith<ExampleComponent1, ExampleComponent2>();

        std::unordered_map<int, ExampleComponent1>& exampleComponents1 = Crown::ComponentManager<ExampleComponent1>::GetComponents();
        std::unordered_map<int, ExampleComponent2>& exampleComponents2 = Crown::ComponentManager<ExampleComponent2>::GetComponents();

        for (int entity : entitiesWithComponents)
        {
            LOG("______________");
            LOG(exampleComponents1.at(entity).exampleData);
            LOG(exampleComponents2.at(entity).moreExampleData);
        }
    }

//#ifdef CROWN_PC
    const float cameraSpeed = 2.5f * m_DeltaTimeTemp; // adjust accordingly
    glm::vec3 cameraPosition =


        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            m_CameraPos += cameraSpeed * m_CamDir;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_CameraPos -= cameraSpeed * m_CamDir;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_CameraPos -= glm::normalize(glm::cross(m_CamDir, m_CamUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_CameraPos += glm::normalize(glm::cross(m_CamDir, m_CamUp)) * cameraSpeed;

//#endif
	*/

    m_TempStoredFrameCount++;
    m_TempStoredDeltaTime += a_DeltaTime;
    if (m_TempStoredDeltaTime >= 1.0f)   //means 1 or more seconds have passed since last log
    {
        m_TempStoredDeltaTime /= m_TempStoredFrameCount;    //average delta time
        LOG((m_TempStoredDeltaTime >= 1.0f ? 1.0f / m_TempStoredDeltaTime : std::round(1.0f / m_TempStoredDeltaTime)));
        m_TempStoredFrameCount = 0;
        m_TempStoredDeltaTime = 0.0f;
    }

}

void MultithreadExample::OnRender()
{
    GetRenderer()->BeginFrame(Crown::CameraHelper::GetMainCameraData());
    GetRenderer()->Render();
    GetRenderer()->EndFrame();
}

void MultithreadExample::OnStart()
{
    GetJobManager()->Add<CrowdJob>(Crown::JobType::Gameplay);
    GetJobManager()->Add<MoveJob>(Crown::JobType::Gameplay);
    GetJobManager()->Add<MoveCameraJob>(Crown::JobType::Gameplay);
    GetJobManager()->Add<Crown::RenderJob>(Crown::JobType::Renderer);
	
    m_CameraComponentSystem = GetComponentSystemManager()->GetSystem<Crown::CameraComponentSystem>();
	
    //CameraComponentSystem
    {
        auto entity = Crown::Entity::Create();

    	//Transform Component
        Crown::TransformComponent transformComponent
        {
            glm::vec3(0.0f, 3.0f, 20.0f),
            glm::vec3(1),
            glm::vec3(-0.6f, 0, 0)
        };
    	
        Crown::ComponentManager<Crown::TransformComponent>::Add(entity, transformComponent);

    	//Camera Component
        Crown::CameraComponent cameraComponent { };
        Crown::ComponentManager<Crown::CameraComponent>::Add(entity, cameraComponent);
        Crown::CameraHelper::SetMainCameraEntity(entity);
        Crown::CameraHelper::SetProjection(entity, static_cast<float>(m_ViewWidth) / static_cast<float>(m_ViewHeight), 65.0f, 0.01f, 1000.f);
    	
        //Move Component
        MoveCameraComponent moveCameraComponent{ 1 };
        Crown::ComponentManager<MoveCameraComponent>::Add(entity, moveCameraComponent);
    }

    auto groundEntity = Crown::Models::LoadModelEntitiesNew("assets/models/untitled3.gltf");

    for (const auto& entity : groundEntity)
    {
        Crown::TransformHelper::SetLocalScale(Crown::ComponentManager<Crown::TransformComponent>::Get(entity), glm::vec3(0.5f));
        Crown::TransformHelper::SetLocalPosition(Crown::ComponentManager<Crown::TransformComponent>::Get(entity), glm::vec3(12, -2, 15));
        Crown::TransformHelper::SetLocalRotation(Crown::ComponentManager<Crown::TransformComponent>::Get(entity), glm::vec3(1.5f, 0, 0));
    }
	
    const float maxRandomPositionOffset = 10;
    const float halfMaxRandomOffset = maxRandomPositionOffset / 2;
    glm::vec3 centerPosition = glm::vec3(0, 0, 10);

	//Crowd
    for (size_t i = 0; i < 300; i++)
    {
        auto giantEntity = Crown::Models::LoadModelEntity("assets/models/Giant.gltf");
    	
        glm::vec3 randomOffset = glm::vec3(
            Crown::Random() * maxRandomPositionOffset - halfMaxRandomOffset,
            0,
            Crown::Random() * maxRandomPositionOffset - halfMaxRandomOffset);

        Crown::TransformComponent transformComponent = Crown::ComponentManager<Crown::TransformComponent>::Get(giantEntity);

        transformComponent.localPosition = centerPosition + randomOffset;
        transformComponent.localScale = glm::vec3(0.1f);
        transformComponent.localRotation = glm::vec3(1.5f, 0, 0);

        Crown::ComponentManager<Crown::TransformComponent>::Set(giantEntity, transformComponent);

        //Move Component
        MoveComponent moveComponent{ 0.5f };
        Crown::ComponentManager<MoveComponent>::Add(giantEntity, moveComponent);

        //Crowd Component
        CrowdComponent crowdComponent { 0.2f };
        Crown::ComponentManager<CrowdComponent>::Add(giantEntity, crowdComponent);
    }

	//Player
    {
        auto giantEntity = Crown::Models::LoadModelEntity("assets/models/Giant.gltf");

        Crown::TransformComponent transformComponent = Crown::ComponentManager<Crown::TransformComponent>::Get(giantEntity);

        transformComponent.localPosition = centerPosition;
        transformComponent.localScale = glm::vec3(0.4f);
        transformComponent.localRotation = glm::vec3(1.5f, 0, 0);

        Crown::ComponentManager<Crown::TransformComponent>::Set(giantEntity, transformComponent);

        //Move Component
        MoveComponent moveComponent{ 0.5f };
        Crown::ComponentManager<MoveComponent>::Add(giantEntity, moveComponent);
    }
}

void MultithreadExample::OnLateUpdate(float a_DeltaTime)
{
}

void MultithreadExample::OnFixedUpdate()
{
}

void MultithreadExample::OnImGuiRender()
{
}
