/** \file engineApp.cpp
*/
#pragma once

#include "include/independent/core/entrypoint.h"
//#include "EditorLayer.h"
#include "../include/ImGuiLayer.h"
#include "engine.h"
/**
\class EditorApp
* \brief entry class to run the application
*/
class EditorApp : public Engine::Application
{
public:
	EditorApp();	//!< constructor
	~EditorApp() override; //!< destructor

private:
	std::shared_ptr<Engine::ImGuiLayer> m_imguiLayer;
	//std::shared_ptr<EditorLayer> m_editorLayer;
};
