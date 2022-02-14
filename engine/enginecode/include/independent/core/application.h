/*! \mainpage IMAT3904 - Game Engine
*
* This program demonstrates the use of rendering techniques using OpenGL and GLFW.
* The program currently has a 2D batch renderer and a normal 3D renderer
* Along with the renderer, the project has multiple perspective cameras with different controls
*
* Control for the camera:
*	- Press '1' Key - Rotation Camera
	- Press '2' Key- First Person Camera
	- W,A,S,D - directional Movement and Mouse controls
	- Space - Toggle between First Person Camera and Follow Camera
	- Follow Camera - W,A,S,D direction movement | Z,X - Up/Down Movement | Q,E - Rotate follow camera
*
* Text at the bottom tells the active camera type
* 
* FPS Counter at the top left - FPS Capped at 60
*/

/** \file application.h
*/
#pragma once


#include "systems/log.h"
#include "systems/physics.h"
#include "events/events.h"
#include "core/timer.h"
#include "core/window.h"
#include "core/inputPoller.h"
#include "core/LayerStack.h"
#include "camera/Camera.h"
#include "camera/FollowPlayer.h"
#include <entt/entt.hpp>


namespace Engine {

	/**
	\class Application
	* \brief Fundemental class of the engine. A singleton which runs the game loop infinitely.
	* Contains the code to render object to the screen
	*/
	class Application
	{
	protected:
		Application(); //!< Constructor

		std::shared_ptr<Log> m_loggerSystem; //!< logger system var
		std::shared_ptr<System> m_windowSystem; //!< window system

		std::shared_ptr<PhysicsSystem> m_physics; // Physics Sytem

		std::shared_ptr<Window> m_window; //!< timer var
		std::shared_ptr<Timer> m_timer; //!< timer var

		EventHandler m_eventHandler; //!< Event handler
		
		bool onWindowClose(WindowCloseEvent& e); //!<run when window closes
		bool onWindowResize(WindowResizeEvent& e); //!<run when resizes
		bool onWindowMoved(WindowMovedEvent& e); //!<run when window is moved
		bool onWindowFocus(WindowFocusEvent& e); //!<run when window has focus
		bool onWindowLostFocus(WindowLostFocusEvent& e); //!<run when window loses focus

		bool onKeyPressed(KeyPressedEvent& e); //!<run when keyboard button is pressed
		bool onKeyReleased(KeyReleasedEvent& e); //!<run when keyboard button is released

		bool onMouseMoved(MouseMovedEvent& e); //!<run when mouse is moved
		bool onMouseBtnPressed(MouseButtonPressedEvent& e); //!<run when mouse button is pressed
		bool onMouseBtnReleased(MouseButtonReleasedEvent& e); //!<run when mouse button is released
		bool onMouseScrolled(MouseScrollEvent& e); //!<run when mouse button is released

	private:
		static Application* s_instance;		//!< Singleton instance of the application
		bool m_running = true;	//!< Is the application running
		glm::mat4 projection;		//<! 3D camera projection

		glm::mat4 models[7];	//!< position information for all the 3D models

		Camera m_camera;	//!< camera for the window
		bool isPlayerCam = false;	//!< is the player camera active
		bool autoCam = false;		//!< bool to toggle auto rotate camera 
		float m_SpeedMultiplier = 2.5f; //!< speed multiplier for follow camera
		float m_cameraSpeed = 1.f; //!< speed of the camera
		const float m_SpeedMultiplierFactor = 0.1f; //!< speed multiplier for follow camera
		
		std::shared_ptr<FollowPlayer> m_playerCam;	//!< follow player camera

		rp3d::PhysicsWorld* m_world; //!< Game world

		bool m_isFullscreen = false; //!< is window fullscreen
		bool m_fullscreenSet = false; //!< is window fullscreen
		char m_setFullScreen; //!< input for full screen
	public:
		virtual ~Application(); //!< Deconstructor
		inline static Application& getInstance() { return *s_instance; } //!< Instance getter from singleton pattern
		inline std::shared_ptr<Window> getAppWindow() { return m_window; };

		inline rp3d::PhysicsWorld* GetWorld() { return m_physics->m_world; };
		inline rp3d::PhysicsCommon& GetPhysCommon() { return m_physics->GetPhysCommon(); }
		void run(); //!< Main loop
		LayerStack m_layerStack;

		entt::registry m_registry; //ESC registry whatever that is
		std::vector<entt::entity> m_entities; //Entities
	};

	// To be defined in users code
	Application* startApplication(); //!< Function definition which provides an entry hook
}