#include "GLFW_GL_Mock.h"

static GLFW_GL_Mock s_mock;

GLFW_GL_Mock::GLFW_GL_Mock()
{
	Engine::WindowProperties properties;

	glfwSystem.reset(new Engine::GLFWSystem);
	glfwSystem->start();

	glfwWindow.reset(new Engine::GLFW_Window(properties));
}

GLFW_GL_Mock::~GLFW_GL_Mock()
{
	glfwWindow->close();
	glfwSystem->stop();
}
