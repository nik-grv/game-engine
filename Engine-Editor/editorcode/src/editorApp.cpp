/*! \file editorApp.cpp
* \brief Main file for the Editor
*
* Contains the entry point of the application
*/
//
#include "../include/editorApp.h"

EditorApp::EditorApp()
{
	m_layerStack.Push(new Engine::ImGuiLayer("ImGUI Layer"));
}

EditorApp::~EditorApp()
{

}

EditorApp::Application* Engine::startApplication()
{
	return new EditorApp();
}