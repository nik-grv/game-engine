/*! \file editorApp.cpp
* \brief Main file for the Editor
*
* Contains the entry point of the application
*/
//
#include "../include/editorApp.h"
namespace Engine
{
	EditorApp::EditorApp()
	{
		Log::e_info("Starting Application...");
		Application& app = Application::getInstance();
		glfwSetWindowTitle(((GLFWwindow*)app.getAppWindow()->getNativewindow()), "Tankery Editor V1");
		EditorLayer::init();

		m_layerStack.Push(new Engine::ImGuiLayer("ImGUI Layer"));

  
}

	EditorApp::~EditorApp()
	{

	}

	EditorApp::Application* Engine::startApplication()
	{
		return new EditorApp();
	}
}