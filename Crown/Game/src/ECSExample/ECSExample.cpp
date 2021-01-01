#include "ECSExample.h"
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
#include <cmath>

#ifdef ECSEXAMPLE
#include "EntryPoint.h"
Crown::Application* Crown::CreateApplication()
{
    return new ECSExample();
}
#endif

void ECSExample::OnUpdate(float a_DeltaTime)
{
    m_XRotation += m_RotateSpeed * a_DeltaTime;
    m_YRotation += (m_RotateSpeed * a_DeltaTime) * 2;
    m_ZRotation += m_RotateSpeed * a_DeltaTime;
	
    m_TransformComponentSystem->SetLocalRotations(m_Entities, glm::vec3(m_XRotation, m_YRotation, m_ZRotation));

    const float moveSpeed = m_BaseMoveSpeed * a_DeltaTime;
    const float rotateSpeed = m_BaseMoveSpeed * a_DeltaTime;
    /*

#ifdef CROWN_PC
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
	
#endif

	*/
	//Calculate & log average framerate every 1 second.
    m_TempStoredFrameCount++;
    m_TempStoredDeltaTime += a_DeltaTime;
	if(m_TempStoredDeltaTime >= 1.0f)   //means 1 or more seconds have passed since last log
	{
        m_TempStoredDeltaTime /= m_TempStoredFrameCount;    //average delta time
		LOG((m_TempStoredDeltaTime >= 1.0f ? 1.0f / m_TempStoredDeltaTime : std::round(1.0f / m_TempStoredDeltaTime)));
        m_TempStoredFrameCount = 0;
        m_TempStoredDeltaTime = 0.0f;
	}
	
    //Temporary fps count
	//LOG(1.0f / a_DeltaTime);
}

void ECSExample::OnRender()
{
    GetRenderer()->BeginFrame(m_CameraComponentSystem->GetMainCameraData());
    GetRenderer()->Render();
    GetRenderer()->EndFrame();
}

void ECSExample::OnStart()
{
    m_CameraComponentSystem = GetComponentSystemManager()->GetSystem<Crown::CameraComponentSystem>();
    m_TransformComponentSystem = GetComponentSystemManager()->GetSystem<Crown::TransformComponentSystem>();
    m_RenderComponentSystem = GetComponentSystemManager()->GetSystem<Crown::RenderComponentSystem>();

	//CameraComponentSystem
    {
        auto entity = Crown::Entity::Create();
        entity.AddToSystem<Crown::TransformComponentSystem>();
        entity.AddToSystem<Crown::CameraComponentSystem>();

        m_CameraComponentSystem->SetMainCameraEntity(entity);
        m_CameraComponentSystem->SetProjection(entity, static_cast<float>(m_ViewWidth) / static_cast<float>(m_ViewHeight), 65.0f, 0.01f, 1000.f);
    	
        m_TransformComponentSystem->SetLocalPosition(entity, glm::vec3(0.0f, 0.0f, 0.0f));
        //m_TransformComponentSystem->SetLocalPosition(entity, glm::vec3(0.0f, 0.0f, 3.0f));
        //m_TransformComponentSystem->SetLookAt(entity, glm::vec3(0));
        m_TransformComponentSystem->SetLookAt(entity, glm::vec3(0, 0, 1));
    }
	
    const float maxRandomPositionOffset = 1;
    const float halfMaxRandomOffset = maxRandomPositionOffset / 2;
	
    for (size_t i = 0; i < 10; i++)
    {
        //auto entities = Crown::Models::LoadModelEntities("assets/models/CrowdKings_MockLvl.glb");                     //binary gltf
        auto entities = Crown::Models::LoadModelEntities("assets/models/Giant.gltf");         //ascii gltf
        //auto entities = Crown::Models::LoadModelEntities("assets/models/untitled.gltf");         //ascii gltf
    	
        for (const auto& entity : entities)
        {
            glm::vec3 randomOffset = glm::vec3(
                Crown::Random() * maxRandomPositionOffset - halfMaxRandomOffset, 
                Crown::Random() * maxRandomPositionOffset - halfMaxRandomOffset, 
                Crown::Random() * maxRandomPositionOffset - halfMaxRandomOffset);
            randomOffset = -randomOffset;
        	
            m_TransformComponentSystem = entity.GetSystem<Crown::TransformComponentSystem>();
        	
            //m_TransformComponentSystem->SetLocalPosition(entity, glm::vec3(0, 0, 70));
            //m_TransformComponentSystem->SetLocalPosition(entity, glm::vec3(0, 0, -1));       //fish
            //m_TransformComponentSystem->SetLocalPosition(entity, glm::vec3(0, 0, 3.1));     //avocado
            m_TransformComponentSystem->SetLocalPosition(entity, glm::vec3(randomOffset.x, randomOffset.y, randomOffset.z - 1.5f));
            m_TransformComponentSystem->SetLocalScale(entity, glm::vec3(0.1f, 0.1f, 0.1f));
            m_Entities.push_back(entity);
        }
    }

    for (size_t i = 0; i < 1; i++)
    {
        //auto entities = Crown::Models::LoadModelEntities("assets/models/CrowdKings_MockLvl.glb");                     //binary gltf
        //auto entities = Crown::Models::LoadModelEntities("assets/models/BarramundiFish.gltf");         //ascii gltf
        auto entities = Crown::Models::LoadModelEntities("assets/models/untitled3.gltf");         //ascii gltf

        for (const auto& entity : entities)
        {
            glm::vec3 randomOffset = glm::vec3(
                Crown::Random() * maxRandomPositionOffset - halfMaxRandomOffset,
                Crown::Random() * maxRandomPositionOffset - halfMaxRandomOffset,
                Crown::Random() * maxRandomPositionOffset - halfMaxRandomOffset);

            m_TransformComponentSystem = entity.GetSystem<Crown::TransformComponentSystem>();

            m_TransformComponentSystem->SetLocalPosition(entity, glm::vec3(0, 0, -70));
            //m_TransformComponentSystem->SetLocalPosition(entity, glm::vec3(0, 0, 100));       //fish
            //m_TransformComponentSystem->SetLocalPosition(entity, glm::vec3(0, 0, 3.1));     //avocado
            //m_TransformComponentSystem->SetLocalPosition(entity, glm::vec3(randomOffset.x, randomOffset.y, randomOffset.z));
            m_TransformComponentSystem->SetLocalScale(entity, glm::vec3(1, 1, 1));

            m_TransformComponentSystem->SetLocalRotation(entity, glm::vec3(180, 180, 1));
        	
            //m_TransformComponentSystem.setr
        	
            m_Entities.push_back(entity);
        }
    }

	
	/*
<<<<
    auto model = GetResourceManager()->Get<Crown::Model>("Box");
	
    {
		const Crown::Entity entity = Crown::Entity::Create();
        m_TransformComponentSystem = entity.AddToSystem<Crown::TransformComponentSystem>();
        m_TransformComponentSystem->SetLocalPosition(entity, glm::vec3(2.0f, 0.0f, -2.0f));
        m_TransformComponentSystem->SetLocalScale(entity, glm::vec3(1.0f, 1.0f, 1.0f));
        m_RenderComponentSystem = entity.AddToSystem<Crown::RenderComponentSystem>();
        //m_RenderComponentSystem->SetModel(entity, model);
        m_Entities.push_back(entity);
    }

    {
		const Crown::Entity entity = Crown::Entity::Create();
        m_TransformComponentSystem = entity.AddToSystem<Crown::TransformComponentSystem>();
        m_RenderComponentSystem = entity.AddToSystem<Crown::RenderComponentSystem>();
        //m_RenderComponentSystem->SetModel(entity, model);
        m_TransformComponentSystem->SetLocalPosition(entity, glm::vec3(-1.5f, -2.2f, -2.5f));
        m_Entities.push_back(entity);
    }

    {
		const Crown::Entity entity = Crown::Entity::Create();
        m_TransformComponentSystem = entity.AddToSystem<Crown::TransformComponentSystem>();
        m_RenderComponentSystem = entity.AddToSystem<Crown::RenderComponentSystem>();
        //m_RenderComponentSystem->SetModel(entity, model);
        m_TransformComponentSystem->SetLocalPosition(entity, glm::vec3(-3.8f, -2.0f, -12.3f));
        m_Entities.push_back(entity);
    }
	*/
}

void ECSExample::OnLateUpdate(float a_DeltaTime)
{
}

void ECSExample::OnFixedUpdate()
{
}

void ECSExample::OnImGuiRender()
{
}
