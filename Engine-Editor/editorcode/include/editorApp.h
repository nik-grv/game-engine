/** \file engineApp.cpp
*/
#pragma once

#include "include/independent/core/entrypoint.h"
//#include "EditorLayer.h"
#include "../include/ImGuiLayer.h"
#include "systems/log.h"
#include "engine_pch.h"
/**
\class EditorApp
* \brief entry class to run the application
*/
namespace Engine
{
	class EditorApp : public Engine::Application
	{
	public:
		EditorApp();	//!< constructor
		~EditorApp() override; //!< destructor
		std::shared_ptr<Log> m_loggerSystem; //!< logger system var
	private:
		std::shared_ptr<Engine::ImGuiLayer> m_imguiLayer;
		//std::shared_ptr<EditorLayer> m_editorLayer;
	};
}