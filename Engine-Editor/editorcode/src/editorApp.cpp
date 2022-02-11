/*! \file editorApp.cpp
* \brief Main file for the Editor
*
* Contains the entry point of the application
*/

#include "engine.h"
#include "../editorcode/include/editorApp.h"
namespace Engine {

	EditorApp::EditorApp()
	{
		//m_layerStack.Push(new EditorLayer("Editor"));
		m_layerStack.Push(new ImGuiLayer());

	}

	EditorApp::~EditorApp()
	{

	}

	Application* startApplication()
	{
		return new EditorApp();
	}


}