 /*! \file engineApp.cpp
* \brief Main file for the Game
*
* Contains the entry point of the application
*/

#include "engineApp.h"
#include "AssimpTestLayer.h"

//! Empty Constructor
/*!
*/
engineApp::engineApp()
{
	m_layerStack.Push(new SceneLayer("Scene Layer"));
	m_layerStack.Push(new UILayer("UI Layer"));
	m_layerStack.Push(new AssimpLayer("Test"));
}

//! Empty Destructor
/*!
*/
engineApp::~engineApp()
{

}

//! Start the appplication
/*!
*/
Engine::Application* Engine::startApplication()
{
	return new engineApp();
}