#include "CrownPCH.h"

#include <GLFW/glfw3.h>

#include "PCWindow.h"
#include "Graphics.h"

#include "Core/Input/Gamepad.h"
#include "Core/Input/Keyboard.h"
#include "Core/Input/Mouse.h"

#include <iostream>

#include "Core/Application.h"
#include "Core/Event/Event.h"
#include "Core/Input/Input.h"

namespace Crown {

	static void error_callback(int a_Error, const char* a_Description)
	{
		fprintf(stderr, "Error %d: %s\n", a_Error, a_Description);
	}

	static void key_callback(GLFWwindow* a_Window, int a_Key, int a_Scancode, int a_Action, int a_Mods)
	{
		auto& keyboard = Application::Get().GetInput()->GetKeyboard();
		if (a_Action == GLFW_PRESS)
			keyboard.PressKey(a_Key);
		else if (a_Action == GLFW_RELEASE)
			keyboard.ReleaseKey(a_Key);

		if (a_Key == GLFW_KEY_ESCAPE && a_Action == GLFW_PRESS)
			glfwSetWindowShouldClose(a_Window, GLFW_TRUE);
	}

	static void scroll_callback(GLFWwindow* a_Window, double a_Xoffset, double a_Yoffset)
	{
		auto& mouse= Application::Get().GetInput()->GetMouse();
		mouse.UpdateScroll(a_Xoffset, a_Yoffset);
	}

	static void cursor_pos_callback(GLFWwindow* a_Window, double a_Xpos, double a_Ypos)
	{
		auto& mouse = Application::Get().GetInput()->GetMouse();
		mouse.UpdateCoords(a_Xpos, a_Ypos);
	}

	static void mouse_button_callback(GLFWwindow* a_Window, int a_Button, int a_Action, int a_Mods)
	{
		auto& mouse = Application::Get().GetInput()->GetMouse();
		if (a_Action == GLFW_PRESS)
			mouse.PressButton(static_cast<MouseButton>(a_Button));
		else if (a_Action == GLFW_RELEASE)
			mouse.ReleaseButton(static_cast<MouseButton>(a_Button));
	}

	static void cursor_enter_callback(GLFWwindow* a_Window, int a_Entered)
	{
		if (a_Entered)
		{
			LOG("Cursor entered window");
		}
		else
		{
			LOG("Cursor left window");
		}
	}

	static void joystick_callback(int a_Jid, int a_Event)
	{
		auto& gamepad = Application::Get().GetInput()->GetGamepad(a_Jid);
		if (a_Event == GLFW_CONNECTED) {
			std::cout << "Joystick " << a_Jid << " connected." << std::endl;
		}
		else {
			std::cout << "Joystick " << a_Jid << " disconnected." << std::endl;
		}
	}

	static void character_callback(GLFWwindow* a_Window, unsigned int a_Codepoint)
	{
		LOG("Character entered");
	}

	void window_close_callback(GLFWwindow* a_Window)
	{
		LOG("Window Closed");
	}

	void window_size_callback(GLFWwindow* a_Window, int a_Width, int a_Height)
	{
		LOG("Window resized");
	}

	static void framebuffer_size_callback(GLFWwindow* a_Window, int a_Width, int a_Height)
	{
		
	}

	void window_content_scale_callback(GLFWwindow* a_Window, float a_Xscale, float a_Yscale)
	{
		LOG("Content scaled");
	}

	void window_pos_callback(GLFWwindow* a_Window, int a_Xpos, int a_Ypos)
	{
		LOG("Window Moved");
	}

	void window_iconify_callback(GLFWwindow* a_Window, int a_Iconified)
	{
		if (a_Iconified)
		{
			LOG("Window Minimized");
		}
		else
		{
			LOG("Window Restored");
		}
	}

	void window_maximize_callback(GLFWwindow* a_Window, int a_Maximized)
	{
		if (a_Maximized)
		{
			LOG("Window Maximized");
		}
		else
		{
			LOG("Window Un-Maximized");
		}
	}

	void window_focus_callback(GLFWwindow* a_Window, int a_Focused)
	{
		if (a_Focused)
		{
			LOG("Window focused");
		}
		else
		{
			LOG("Window unfocused");
		}
	}

	void drop_callback(GLFWwindow* a_Window, int a_Count, const char** a_Paths)
	{
		LOG("Path dropped");
	}

	void monitor_callback(GLFWmonitor* a_Monitor, int a_Event) {

	}

	void fatal_error(const std::string& a_ErrorString)
	{
		perror(a_ErrorString.c_str());
		glfwTerminate();
		system("PAUSE");
		exit(1);
	}


	PCWindow::PCWindow()
	{
		glfwSetErrorCallback(error_callback);
	}

	PCWindow::~PCWindow()
	{
		glfwDestroyWindow(static_cast<GLFWwindow*>(m_Window));
		glfwTerminate();
	}

	void PCWindow::Init(const char* a_title, int a_width, int a_height)
	{
		if (!glfwInit())
			exit(EXIT_FAILURE);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

		const auto window = glfwCreateWindow(a_width, a_height, a_title, nullptr, nullptr);
		if (!window) {
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		this->m_Window = window;

		glfwGetFramebufferSize(window, &m_Width, &m_Height);

		// auto cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
		// glfwSetCursor(window, cursor);
		// glfwSetCursor(window.window, NULL);
		// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// glfwDestroyCursor(cursor);

		// unsigned char pixels[16 * 16 * 4];
		// memset(pixels, 0xff, sizeof(pixels));

		// GLFWimage image;
		// image.width = 16;
		// image.height = 16;
		// image.pixels = pixels;

		// GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);
		// glfwSetCursor(window.window, cursor);

		// if (glfwRawMouseMotionSupported())
		//     glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

		glfwSetKeyCallback(window, key_callback);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetCursorPosCallback(window, cursor_pos_callback);
		glfwSetMonitorCallback(monitor_callback);
		glfwSetCursorEnterCallback(window, cursor_enter_callback);
		glfwSetJoystickCallback(joystick_callback);
		glfwSetWindowCloseCallback(window, window_close_callback);
		glfwSetWindowSizeCallback(window, window_size_callback);
		glfwSetCharCallback(window, character_callback);
		glfwSetDropCallback(window, drop_callback);
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
		{
			fatal_error("Failed to initialize GLAD");
		}

		glfwSwapInterval(1);

		CHECK_GL();
	}

	bool PCWindow::IsClosing() const
	{
		return glfwWindowShouldClose(static_cast<GLFWwindow*>(m_Window));
	}

	void PCWindow::NewFrame()
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void PCWindow::Present()
	{
		glfwSwapBuffers(static_cast<GLFWwindow*>(m_Window));
	}

	void PCWindow::GetSize(int& a_width, int& a_height)
	{
		glfwGetFramebufferSize(static_cast<GLFWwindow*>(m_Window), &a_width, &a_height);
		m_Width = a_width;
		m_Height = a_height;
	}

	void PCWindow::Resize(int a_width, int a_height)
	{

	}

	bool PCWindow::GetIsFullscreen()
	{
		return false;
	}

	void PCWindow::SetFullscreen(bool a_IsFullscreen)
	{
		
	}

	void PCWindow::SetMouseCoords(double a_X, double a_Y) const
	{
		glfwSetCursorPos(static_cast<GLFWwindow*>(m_Window), a_X, a_Y);
	}
}
