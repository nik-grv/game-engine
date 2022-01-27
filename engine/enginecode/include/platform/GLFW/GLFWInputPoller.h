/** \file GLFWInputPoller.h
*/
#pragma once

#include "glm/glm.hpp"
#include <GLFW/glfw3.h>

namespace Engine {

	/**
	\class GLFWInputPoller
	*\brief API Agnostic input poller for getting keyboard and mouse states - specific to the GLFW
	*/
	class GLFWInputPoller
	{
	public:
		static bool isKeyPressed(int32_t keyCode);//!< is the key pressed
		static bool isKeyReleased(int32_t keyCode);//!< is the key released
		static bool isMouseButtonPressed(int32_t button);//!< is the mouse button pressed
		static glm::vec2 getMousePosition(); //!< get mouse position
		static void setCurrentWindow(GLFWwindow* newWindow) { s_window = newWindow; } //!< set current window
	private:
		static GLFWwindow* s_window; //!< pointer to native GLFWwindow
	};
}