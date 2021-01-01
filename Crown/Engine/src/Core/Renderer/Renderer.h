#pragma once
///Example class
#include <memory>
#include <glm/glm.hpp>

namespace  Crown
{
	struct ModelData;
	struct RenderComponent;
	struct CameraComponent;

	//class Window;
	enum class RenderAPI
	{
	    OpenGL,
	    Gnm
	};

	class Renderer
	{
	public:
		Renderer() = default;
		virtual ~Renderer() = default;

	    virtual void Init() = 0;

	    //To be called before IMGUI function calls
	    //TODO: add parameters e.g. camera, world
	    virtual void BeginFrame(const CameraComponent& a_CameraComponent) = 0;
		virtual void Submit(const RenderComponent& a_RenderComponent) = 0; //TODO add vector of model ref and vector of material ref;
	    virtual void Render() = 0;
		virtual void EndFrame() = 0;
		
	    //Renderer will only support one window
	    //virtual Crown::Window& GetWindow() = 0;

	    virtual RenderAPI GetRenderAPI() = 0;

		static std::shared_ptr<Renderer> Create();


	};
}
