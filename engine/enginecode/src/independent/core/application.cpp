/** \file application.cpp
*\brief The Application Class file
* Contains all the code for creating the window, rendering object and camera movements
*/
#include "engine_pch.h"
#include <glad/glad.h>
#include <cassert>
#include "core/application.h"

#ifdef NG_PLATFORM_WINDOWS
#include "platform/GLFW/GLFWSystem.h"
#endif

#include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/type_ptr.hpp>


#include "rendering/subTexture.h"

#include "rendering/IndexBuffer.h"
#include "rendering/VertexBuffer.h"
#include "rendering/VertexArray.h"
#include "rendering/ShaderRend.h"
#include "rendering/UniformBuffer.h"

#include "rendering/Renderer3D.h"
#include "rendering/Renderer2D.h"

#include "../../Engine-Editor/editorcode/include/ImGuiHelper.h";

namespace Engine {
	// Set static vars
	Application* Application::s_instance = nullptr;

	//! Constructor to setup the window and the keyboard events 
	/*!
	*/
	Application::Application()
	{
		for (auto ent : m_entities)
			ent = m_registry.create();
		
		if (s_instance == nullptr)
		{
			s_instance = this;
		}
		//Start system and logger
		m_loggerSystem.reset(new Log); 
		m_loggerSystem->start();

		//start windows system
#ifdef NG_PLATFORM_WINDOWS
		m_windowSystem.reset(new GLFWSystem);
#endif
		m_windowSystem->start();

		m_physics.reset(new PhysicsSystem);
		m_physics->start(); // reset first? we need?
		//m_physics->m_world->setGravity(reactphysics3d::Vector3(0.f, -10.f, 0.f));
		//m_physics->m_world->setIsGravityEnabled(true);


		//reset timer
		m_timer.reset(new ChronoTimer);
		m_timer->start();


#pragma region [FEATURE] Set Fullsceen
		/*while (!m_fullscreenSet)
		{
			std::cout << "\nStart with Fullscreen (Y/N) ?\n";
			std::cin >> m_setFullScreen;
			
			while (tolower(m_setFullScreen) != 'y' && tolower(m_setFullScreen) != 'n')
			{
				std::cout << "\nPlease enter Y or N as ur choice : ";
				std::cin >> m_setFullScreen;
			}

			if (tolower(m_setFullScreen) == 'y')
			{
				m_isFullscreen = true;
				m_fullscreenSet = true;
				std::cout << "\nLaunching in Fullscreen Mode!\n";
				std::cin.ignore();
			}
			else
			{
				m_isFullscreen = false;
				m_fullscreenSet = true;
				std::cout << "\nLaunching in Minimised Mode!\n";
				std::cin.ignore();
			}
		
		}
		*/
#pragma endregion

		WindowProperties props("My Game Engine",RendererShared::SCR_WIDTH, RendererShared::SCR_HEIGHT,m_isFullscreen);
		m_window.reset(Window::create(props));

		//window callbacks
		m_window->getEventHandler().setOnCloseCallback(std::bind(&Application::onWindowClose, this, std::placeholders::_1));
		m_window->getEventHandler().setOnResizeCallback(std::bind(&Application::onWindowResize, this, std::placeholders::_1));
		m_window->getEventHandler().setOnMovedCallback(std::bind(&Application::onWindowMoved, this, std::placeholders::_1));
		m_window->getEventHandler().setOnLostFocusCallback(std::bind(&Application::onWindowLostFocus, this, std::placeholders::_1));
		m_window->getEventHandler().setOnFocusCallback(std::bind(&Application::onWindowFocus, this, std::placeholders::_1));
		//keyboard callbacks
		m_window->getEventHandler().setOnKeyPressedEvent(std::bind(&Application::onKeyPressed, this, std::placeholders::_1));
		m_window->getEventHandler().setOnKeyReleasedEvent(std::bind(&Application::onKeyReleased, this, std::placeholders::_1));
		
		//mouse callbacks
		m_window->getEventHandler().setOnMouseMovedEvent(std::bind(&Application::onMouseMoved, this, std::placeholders::_1));
		m_window->getEventHandler().setOnMouseBtnPressedEvent(std::bind(&Application::onMouseBtnPressed, this, std::placeholders::_1));
		m_window->getEventHandler().setOnMouseBtnReleasedEvent(std::bind(&Application::onMouseBtnReleased, this, std::placeholders::_1));
		m_window->getEventHandler().setOnMouseScrolledEvent(std::bind(&Application::onMouseScrolled, this, std::placeholders::_1));

		InputPoller::setNativeWindow(m_window->getNativewindow());
		m_timer->reset();

		//ImGuiHelper::init();

		Renderer2D::init();
		//Renderer3D::init();

	}

#pragma region AppEvents
	/*!
	\param closeEvent WindowCloseEvent  - window close event
	*/
	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		e.handle(true);
		m_running = false;
		return e.isHandled();
	}

	/*!
	\param resizeEvent WindowResizeEvent  - window resize event
	*/
	bool Application::onWindowResize(WindowResizeEvent& e)
	{
		e.handle(true);
		auto& size = e.getSize();
	//	Log::trace("Window Resize : {0} / {1} ",size.x,size.y);
		return e.isHandled();
	}


	/*!
	\param movedEvent WindowMovedEvent  - window moved event
	*/
	bool Application::onWindowMoved(WindowMovedEvent& e)
	{
		e.handle(true);
		auto& size = e.getPos();
		Log::trace("Window Moved : {0} / {1}", size.x, size.y);
		return e.isHandled();
	}


	/*!
	\param focusEvent WindowLostFocusEvent  - window lost focus event
	*/
	bool Application::onWindowLostFocus(WindowLostFocusEvent& e)
	{
		e.handle(true);
		m_layerStack.SetAllActive(false);
		//Log::trace("Lost Focus");
		return e.isHandled();
	}

	/*!
	\param focusEvent WindowFocusEvent  - window gained focus event
	*/
	bool Application::onWindowFocus(WindowFocusEvent& e)
	{
		e.handle(true);
		m_layerStack.SetAllActive(true);
		//Log::trace("Has Focus");
		return e.isHandled();
	}

	/*!
	\param keyEvent KeyPressedEvent  - keyboard key press event
	*/
	bool Application::onKeyPressed(KeyPressedEvent& e)
	{
		e.handle(true);
		//Log::trace("{0} Key Pressed ", e.getKeyCode());
		if (e.getKeyCode() == NG_KEY_ESCAPE)
		{
			glfwSetInputMode(reinterpret_cast<GLFWwindow*>(m_window->getNativewindow()), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		if (e.getKeyCode() == NG_KEY_SPACE)
		{
			//switch to player cam....
			isPlayerCam = !isPlayerCam;
		}

		m_layerStack.onKeyPressed(e);
		return e.isHandled();
	}

	/*!
	\param keyEvent KeyReleasedEvent  - keyboard key released event
	*/
	bool Application::onKeyReleased(KeyReleasedEvent& e)
	{
		e.handle(true);
		//reset speed multiplier for both cameras on key release
		if (e.getKeyCode() == NG_KEY_W || e.getKeyCode() == NG_KEY_A || e.getKeyCode() == NG_KEY_S || e.getKeyCode() == NG_KEY_D
			|| e.getKeyCode() == NG_KEY_Q || e.getKeyCode() == NG_KEY_E || e.getKeyCode() == NG_KEY_Z || e.getKeyCode() == NG_KEY_X)
			m_SpeedMultiplier = 2.5f;

		m_layerStack.onKeyReleased(e);

		return e.isHandled();
	}

	/*!
	\param mouseEvent MouseMovedEvent  - mouse moved event
	*/
	bool Application::onMouseMoved(MouseMovedEvent& e)
	{
		e.handle(true);
		auto& size = e.getMousePos();
		//m_camera.mouseMovement(size.x, size.y);
		//Log::trace("Mouse Moved : {0} / {1}", size.x, size.y);
		m_layerStack.onMouseMoved(e);

		return e.isHandled();
	}

	/*!
	\param mouseEvent MouseButtonPressedEvent  - mouse button press event
	*/
	bool Application::onMouseBtnPressed(MouseButtonPressedEvent& e)
	{
		e.handle(true);
		if (e.getButton() == 0)
		{
			//glfwSetInputMode(reinterpret_cast<GLFWwindow*>(m_window->getNativewindow()), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		m_layerStack.onMouseBtnPressed(e);

		return e.isHandled();
	}

	/*!
	\param mouseEvent MouseButtonReleasedEvent  - mouse button release event
	*/
	bool Application::onMouseBtnReleased(MouseButtonReleasedEvent& e)
	{
		e.handle(true);
		//Log::trace("Mouse Button Released : {0} ", e.getButton());
		m_layerStack.onMouseBtnReleased(e);

		return e.isHandled();
	}

	/*!
	\param mouseEvent MouseScrollEvent  - mouse scroll event
	*/
	bool Application::onMouseScrolled(MouseScrollEvent& e)
	{
		e.handle(true);
		m_camera.mouseScroll(e.getYScroll());
		projection =
			glm::perspective(glm::radians(m_camera.getFOV()), (float)RendererShared::SCR_WIDTH / (float)RendererShared::SCR_HEIGHT, 0.1f, 100.f);

		m_layerStack.onMouseScrolled(e);

		return e.isHandled();
	}

#pragma endregion

	Application::~Application()
	{

		//delete world
		m_physics->stop();
		//stop the systems and logger
		m_loggerSystem->stop();
		
		//ImGuiHelper::shutdown();

		//stop windows system
		m_windowSystem->stop();
		
		for (auto ent : m_entities)
			m_registry.destroy(ent);

	}


	//! Defines the main loop functions of the application. 
	//! Code for rendering, camera movement, etc run in the function
	/*!
	*/
	void Application::run()
	{
#pragma endregion RenderCommands
		float timestep = 0.f;
		while (m_running)
		{
#pragma region [History] - While Loop

#pragma endregion
			timestep = m_timer->getElapsedTime();
			m_timer->reset();

			m_physics->m_world->update(timestep);
			m_layerStack.Update(timestep);
			m_layerStack.Render();
			m_window->onUpdate(timestep);
		}

	}
}
