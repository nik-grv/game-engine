/** \file InputPoller.cpp
*/

#include "engine_pch.h"
#include "core/inputPoller.h"

#ifdef  NG_PLATFORM_WINDOWS
#include "platform/GLFW/GLFWInputPoller.h"
#endif

namespace Engine 
{
#ifdef  NG_PLATFORM_WINDOWS


	/*!
	\param keyCode int32_t - keycode of the key pressed
	*/
	bool InputPoller::isKeyPressed(int32_t keyCode)
	{
		return GLFWInputPoller::isKeyPressed(keyCode);
	}
	
	/*!
	\param keyCode int32_t - keycode of the key released
	*/
	bool InputPoller::isKeyReleased(int32_t keyCode)
	{
		return GLFWInputPoller::isKeyReleased(keyCode);
	}

	/*!
	\param button int32_t - keycode of the button pressed
	*/
	bool InputPoller::isMouseButtonPressed(int32_t button)
	{
		return GLFWInputPoller::isMouseButtonPressed(button);
	}

	glm::vec2 InputPoller::getMousePosition()
	{
		return GLFWInputPoller::getMousePosition();
	}

	/*!
	\param window void* - native window reference
	*/
	void InputPoller::setNativeWindow(void* nativeWin)
	{
		return GLFWInputPoller::setCurrentWindow(reinterpret_cast<GLFWwindow*>(nativeWin));
	}
#endif
}