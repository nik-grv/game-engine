/** \file inputPoller.h
*/
#pragma once

#include "glm/glm.hpp"

namespace Engine {

	/**
	\class InputPoller
	*\brief API Agnostic input poller for getting keyboard and mouse states
	*/
	class InputPoller
	{
	public:
		static bool isKeyPressed(int32_t keyCode);//!< is the key pressed
		static bool isKeyReleased(int32_t keyCode);//!< is the key released
		static bool isMouseButtonPressed(int32_t button);//!< is the mouse button pressed
		static glm::vec2 getMousePosition(); //!< get mouse position
		static void setNativeWindow(void* nativeWin); //!< set native window
		inline static float getMouseX() { return getMousePosition().x; } //!< get Mouse X position
		inline static float getMouseY() { return getMousePosition().y; } //!< get Mouse Y position
	};
}