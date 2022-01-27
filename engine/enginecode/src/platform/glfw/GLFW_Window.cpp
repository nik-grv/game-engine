/** \file GLFW_Window.cpp
*/

#include "engine_pch.h"
#include "platform/GLFW/GLFW_Window.h"
#include "platform/GLFW/GLFW_OpenGL_GC.h"
#include "systems/log.h"

namespace Engine
{
#ifdef NG_PLATFORM_WINDOWS
	Window* Window::create(const WindowProperties& properties)
	{
		return new GLFW_Window(properties);
	}
#endif

	GLFW_Window::GLFW_Window(const WindowProperties& properties)
	{
		init(properties);
	}

	void GLFW_Window::init(const WindowProperties& properties)
	{
		m_properties = properties;

		m_aspectRatio = static_cast<float>(m_properties.width) / static_cast<float> (m_properties.height);

		if (m_properties.isFullScreen)
		{
			m_native = glfwCreateWindow(glfwGetVideoMode(glfwGetPrimaryMonitor())->width, glfwGetVideoMode(glfwGetPrimaryMonitor())->height,
												m_properties.title,glfwGetPrimaryMonitor(), nullptr);
		}
		else
		{
			m_native = glfwCreateWindow(m_properties.width, m_properties.height, m_properties.title, nullptr, nullptr);
		
		}

		m_graphicsContext.reset(new GLFW_OpenGL_GC(m_native));
		m_graphicsContext->init();

		glfwSetWindowUserPointer(m_native, static_cast<void*>(&m_eventHandler));


		//create custom cursor -------
		/*unsigned char pixels[16 * 16 * 4];
		memset(pixels, 0x0, sizeof(pixels));

		GLFWimage image;
		image.width = 16;
		image.height = 16;
		image.pixels = pixels;*/

		////GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);-------

		GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
		glfwSetCursor(m_native, cursor);
		glfwSetInputMode(m_native, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		//window callbacks
		glfwSetWindowCloseCallback(m_native, [](GLFWwindow* window)
		{
			WindowCloseEvent e;
			EventHandler* eventHandler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			auto& onClose = eventHandler->getOnCloseCallback();
			onClose(e);
		}
		);
		
		glfwSetWindowSizeCallback(m_native, [](GLFWwindow* window,int32_t w, int32_t h)
		{
			WindowResizeEvent e(w,h);
			EventHandler* eventHandler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			auto& onResize = eventHandler->getOnResizeCallback();
			onResize(e);
		}
		);
		
		glfwSetWindowPosCallback(m_native, [](GLFWwindow* window,int32_t x, int32_t y)
		{
			WindowMovedEvent e(x,y);
			EventHandler* eventHandler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			auto& onMoved = eventHandler->getOnMovedCallback();
			onMoved(e);
		}
		);

		glfwSetWindowFocusCallback(m_native, [](GLFWwindow* window, int focused)
		{
			EventHandler* eventHandler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			if (focused == 0)
			{
				WindowLostFocusEvent e;
				auto& onLostFocus = eventHandler->getOnLostFocusCallback();
				onLostFocus(e);
			}
			else if (focused == 1)
			{
				WindowFocusEvent e;
				auto& onFocus = eventHandler->getOnFocusCallback();
				onFocus(e);
			}
		}
		);
		
		//keyboard callbacks
		glfwSetKeyCallback(m_native, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			EventHandler* eventHandler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			if (action == GLFW_PRESS)
			{
				KeyPressedEvent e(key,0);
				auto& onKeyPress = eventHandler->getOnKeyPressedEventCallback();
				onKeyPress(e);
			}
			else if (action == GLFW_REPEAT)
			{
				KeyPressedEvent e(key,1);
				auto& onKeyPress = eventHandler->getOnKeyPressedEventCallback();
				onKeyPress(e);
			}
			else if (action == GLFW_RELEASE)
			{
				KeyReleasedEvent e(key);
				auto& onKeyRelease = eventHandler->getOnKeyReleasedEventCallback();
				onKeyRelease(e);
			}
		}
		);

		//mouse callbacks
		glfwSetCursorPosCallback(m_native, [](GLFWwindow* window,double x, double y)
		{
			MouseMovedEvent e(static_cast<float>(x), static_cast<float>(y));
			EventHandler* eventHandler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			auto& onMouseMoved = eventHandler->getOnMouseMovedEventCallback();
			onMouseMoved(e);
		}
		);
		
		glfwSetMouseButtonCallback(m_native, [](GLFWwindow* window, int button, int action, int modes)
		{
			EventHandler* eventHandler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			if (action == GLFW_PRESS)
			{
				MouseButtonPressedEvent e(button);
				auto& onMouseButtonPress = eventHandler->getOnMouseBtnPressedEventCallback();
				onMouseButtonPress(e);
			}
			else if (action == GLFW_RELEASE)
			{
				MouseButtonReleasedEvent e(button);
				auto& onMouseButtonRelease = eventHandler->getOnMouseBtnReleasedEventCallback();
				onMouseButtonRelease(e);
			}
		}
		);

		glfwSetScrollCallback(m_native, [](GLFWwindow* window, double x, double y)
		{
			MouseScrollEvent e(static_cast<float>(x), static_cast<float>(y));
			EventHandler* eventHandler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			auto& onMouseScrolled = eventHandler->getOnMouseScrolledEventCallback();
			onMouseScrolled(e);
		}
		);

	}

	void GLFW_Window::close()
	{
		glfwDestroyWindow(m_native);
	}

	void GLFW_Window::onUpdate(float timestep)
	{
		glfwPollEvents();
		m_graphicsContext->swapBuffer();
	}

	void GLFW_Window::setvSync(bool vSync)
	{
		m_properties.isVsyncOn = vSync;
		if (m_properties.isVsyncOn)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

	}
}
