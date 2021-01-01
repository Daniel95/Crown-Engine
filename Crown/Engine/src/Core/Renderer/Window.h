#pragma once

namespace Crown
{
	class Window
	{
	public:
		virtual ~Window() = default;
		
	    virtual void Init(const char* a_title, int a_width, int a_height) = 0;

	    //To be called before IMGUI function calls
	    virtual void NewFrame() = 0;
		
	    //To be called once rendered image is ready to be shown to screen
	    virtual void Present() = 0;

		virtual void GetSize(int& a_width, int& a_height) = 0;
	    virtual void Resize(int a_width, int a_height) = 0;
		
		virtual float GetRatio() const = 0;
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;

	    virtual bool GetIsFullscreen() = 0;
	    virtual void SetFullscreen(bool a_isFullscreen) = 0;

		virtual bool IsClosing() const = 0;
		virtual void* GetWindow() const = 0;

		static Window* Create();
	};
}