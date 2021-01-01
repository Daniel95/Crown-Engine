#include "CrownPCH.h"

#include "Application.h"

#include <Platform/PC/Graphics.h>


#include "Core/Renderer/Renderer.h"
#include "Resource/ResourceManager.h"
#include "ECS/System/ComponentSystemManager.h"
#include "Core/SceneManagement/SceneManager.h"
#include "Core/Audio/AudioManager.h"
#include "ECS/Component/TransformComponentSystem.h"
#include "ECS/Component/RenderComponentSystem.h"
#include "ECS/Component/TagComponentSystem.h"
#include "ECS/Component/AudioSourceComponentSystem.h"
#include "Pathfinding/PathMovement.h"

#include "Time/Time.h"
#include "ECS/Component/CameraComponentSystem.h"
#include "ECS/Job/JobManager.h"

namespace Crown
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        ASSERT_ENGINE(!s_Instance, "A instance of Application already exists!");
        s_Instance = this;

        std::srand(0);
    }

	void Application::Run()
	{
		LOG_ENGINE("Application Run");

        m_Window = Window::Create();
        m_Window->Init("Crown", 1280, 720);

        m_Input = std::make_unique<Input>();

        CHECK_GL();
        
		if (!m_UseExternalRenderer)
		{
			m_Renderer = Renderer::Create();
			m_Renderer->Init();

            CHECK_GL();
		}

        m_ResourceManager = std::make_shared<ResourceManager>();
        m_JobManager = std::make_shared<JobManager>();
        m_ComponentSystemManager = std::make_shared<ComponentSystemManager>();
		m_SceneManager = std::make_shared<SceneManager>();
		m_AudioManager = std::make_shared<AudioManager>();
    	
        m_ComponentSystemManager->AddSystem<TransformComponentSystem>();
        m_ComponentSystemManager->AddSystem<RenderComponentSystem>();
        m_ComponentSystemManager->AddSystem<CameraComponentSystem>();
		m_ComponentSystemManager->AddSystem<TagComponentSystem>();
		m_ComponentSystemManager->AddSystem<AudioSourceComponentSystem>();
        m_ComponentSystemManager->AddSystem<PathMovement>();

		OnStart();

        while (m_Running && !m_Window->IsClosing())
        {
            CROWN_PROFILE_FRAME("MainThread");

            m_Input->Update();
            Update();
            //FixedUpdate();
            LateUpdate();
            
            m_Window->NewFrame();
            Render();
            ImGuiRender();
            
            m_Window->Present();
        }
        delete m_Window;
    }

    void Application::Update()
    {
        CROWN_PROFILE_FUNCTION();
    	
        float time = Time::GetTime();
        m_DeltaTime = time - m_LastFrameTime;
        m_LastFrameTime = time;
    	
        m_JobManager->Update(JobType::Gameplay);
        m_ComponentSystemManager->Update(m_DeltaTime);
    	
        OnUpdate(m_DeltaTime);
    }

	/*
    void Application::FixedUpdate()
    {
        OnFixedUpdate();
    }
	*/

    void Application::LateUpdate()
    {
        CROWN_PROFILE_FUNCTION();

        OnLateUpdate(m_DeltaTime);
    }

    void Application::ImGuiRender()
    {
        CROWN_PROFILE_FUNCTION();

        m_ImGuiRenderEvent.Dispatch();
        OnImGuiRender();
    }

    void Application::Render()
    {
        CROWN_PROFILE_FUNCTION();

        m_JobManager->Update(JobType::Renderer);
    	
        OnRender();
		if (!m_UseExternalRenderer)
		{
   //         m_Renderer->BeginFrame();
   //         m_Renderer->EndFrame();
			//m_Renderer->Render();
		}
    }
}
