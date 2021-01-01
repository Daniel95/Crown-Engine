#pragma once
#include "Event/ApplicationEvent.h"
#include "Core/Renderer/Renderer.h"
#include "Renderer/Window.h"

#include <memory>

//#ifdef CROWN_PC
#include "Input/Input.h"
#include "Optick/optick.h"
#include <thread>

//#endif

namespace Crown
{
	class JobManager;
	class Time;
	class ComponentSystemManager;
	class ResourceManager;
	class SceneManager;
	class AudioManager;

    class Application
    {
    public:
        UpdateEvent m_UpdateEvent;
        FixedUpdateEvent m_FixedUpdateEvent;
        LateUpdateEvent m_LateUpdateEvent;
        ImGuiRenderEvent m_ImGuiRenderEvent;
        RenderEvent m_RenderEvent;

        static Application& Get() { return *s_Instance; }

        Application();
        virtual ~Application() = default;
        void Run();

		// Temp functions to avoid OpenGL errors when using SFML
		void UseExternalRenderer(const bool a_UseExternalRenderer) { m_UseExternalRenderer = a_UseExternalRenderer;	};
		bool UsingExternalRenderer() const { return m_UseExternalRenderer; }

		void IsRunning(const bool a_IsRunning) { m_Running = a_IsRunning; };
		bool IsRunning() const { return m_Running; }
		Input* GetInput() const { return m_Input.get(); }
        std::shared_ptr<ResourceManager> GetResourceManager() const { return m_ResourceManager; }
        std::shared_ptr<Renderer> GetRenderer() const { return m_Renderer; }
        std::shared_ptr<ComponentSystemManager> GetComponentSystemManager() const { return m_ComponentSystemManager; }
        std::shared_ptr<JobManager> GetJobManager() const { return m_JobManager; }
		std::shared_ptr<SceneManager> GetSceneManager() const { return m_SceneManager; }
        std::shared_ptr<AudioManager> GetAudioManager() const { return m_AudioManager; }
        const std::thread& GetRenderThread() const { return m_RenderThread; }
        float GetDeltaTime() const { return m_DeltaTime; }

    protected:
        virtual void OnStart() = 0;
        virtual void OnUpdate(float a_DeltaTime) = 0;
        virtual void OnLateUpdate(float a_DeltaTime) = 0;
        virtual void OnFixedUpdate() = 0;
        virtual void OnRender() = 0;
        virtual void OnImGuiRender() = 0;

    private:
        static Application* s_Instance;
        bool m_Running = true;
		bool m_UseExternalRenderer = false;
        float m_LastFrameTime = 0.0f;
        float m_DeltaTime = 0;

        std::shared_ptr<Renderer> m_Renderer;
        std::shared_ptr<ResourceManager> m_ResourceManager;
        std::shared_ptr<ComponentSystemManager> m_ComponentSystemManager;
		std::unique_ptr<Input> m_Input;
		Window* m_Window = nullptr;
        std::shared_ptr<JobManager> m_JobManager;
		std::shared_ptr<SceneManager> m_SceneManager;
        std::shared_ptr<AudioManager> m_AudioManager;
        std::thread m_RenderThread;
    	
        void Update();
        //void FixedUpdate();
        void LateUpdate();
        void ImGuiRender();
        void Render();
    };

    Application* CreateApplication();
}
