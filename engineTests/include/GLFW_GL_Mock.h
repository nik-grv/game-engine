/** \file GLFW_GL_Mock.h*/
#pragma once

#include "include/platform/GLFW/GLFWSystem.h"
#include "include/platform/GLFW/GLFW_Window.h"

/**
	\class GLFW_GL_Mock
	* class to create OpenGLContext for testing
	*/
class GLFW_GL_Mock
{
public:
	GLFW_GL_Mock(); //!< constructor
	~GLFW_GL_Mock(); //!< destructor
	std::shared_ptr<Engine::System> glfwSystem;
	std::shared_ptr<Engine::Window> glfwWindow;
};