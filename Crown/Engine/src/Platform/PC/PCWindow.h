#pragma once

#include "Core/Event/ApplicationEvent.h"
#include "Core/Event/KeyEvent.h"
#include "Core/Renderer/Window.h"

namespace Crown
{
	class PCWindow final : public Window
	{
	public:
		PCWindow();
		~PCWindow();

		// Initialize a window with given title and dimensions
		void Init(const char* a_title, int a_width = 1, int a_height = 1) override;

		// To be called before IMGUI function calls
		void NewFrame() override;
		
		//To be called once rendered image is ready to be shown to screen
		void Present() override;

		// Whether the window is currently closing
		bool IsClosing() const override;
		void* GetWindow() const override { return m_Window; }

		void GetSize(int& a_width, int& a_height) override;
		void Resize(int a_width, int a_height) override; // REVIEW: rename to SetSize?
		
		bool GetIsFullscreen() override; // REVIEW: Rename to IsFullscreen?
		void SetFullscreen(bool a_IsFullscreen) override;

		float GetRatio() const override { return static_cast<float>(m_Width) / static_cast<float>(m_Height); }
		int GetWidth() const override { return m_Width; }
		int GetHeight() const override { return m_Height; }

		void SetMouseCoords(double a_X, double a_Y) const;

	private:
		WindowResizeEvent m_ResizeEvent;
		KeyPressedEvent m_KeyPressedEvent;
		KeyReleasedEvent m_KeyReleasedEvent;
		KeyTypedEvent m_KeyTypedEvent;
		
		void* m_Window = nullptr;
		int m_Width = 0;
		int m_Height = 0;
	};
}

