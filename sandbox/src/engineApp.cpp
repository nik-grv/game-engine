 /*! \file engineApp.cpp
* \brief Main file for the Game
*
* Contains the entry point of the application
*/

#include "engineApp.h"

//! Empty Constructor
/*!
*/
engineApp::engineApp()
{
	Log::s_info("Starting Sandbox...");
	Log::s_info("");
	//m_layerStack.Push(new SceneLayer("Scene Layer"));
	//m_layerStack.Push(new AssimpLayer("Test"));
	//m_layerStack.Push(new EnTTLayer("Entt Layer"));
	m_layerStack.Push(new FramebufferLayer("Framebuffer Layer"));
	//m_layerStack.Push(new UILayer("UI Layer"));
	Log::s_info("== Sandbox Running ==");
	Log::s_info("");
}

//! Empty Destructor
/*!
*/
engineApp::~engineApp()
{

}

//! Start the application
/*!
*/
Engine::Application* Engine::startApplication()
{
	return new engineApp();
}