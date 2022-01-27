/** \file GLFWInputPoller.cpp
*/

#include "engine_pch.h"
#include "platform/GLFW/GLFWInputPoller.h"

namespace Engine
{
	GLFWwindow* GLFWInputPoller::s_window = nullptr;

	bool GLFWInputPoller::isKeyPressed(int32_t keyCode)
	{
		if (s_window)
		{
			auto ans = glfwGetKey(s_window, keyCode);
			return ans == GLFW_PRESS || ans == GLFW_REPEAT;
		}
		return false;
	}
	
	bool GLFWInputPoller::isKeyReleased(int32_t keyCode)
	{
		if (s_window)
		{
			auto ans = glfwGetKey(s_window, keyCode);
			return ans == GLFW_RELEASE;
		}
		return false;
	}
	bool GLFWInputPoller::isMouseButtonPressed(int32_t button)
	{
		if (s_window)
		{
			auto ans = glfwGetMouseButton(s_window, button);
			return ans == GLFW_PRESS;
		}
		return false;
	}
	glm::vec2 GLFWInputPoller::getMousePosition()
	{
		if (s_window)
		{
			double xPos;
			double yPos;
			glfwGetCursorPos(s_window,&xPos,&yPos);
			return glm::vec2(static_cast<float>(xPos), static_cast<float>(yPos));
		}
		return glm::vec2(-1.f,1.f);
	}
}
