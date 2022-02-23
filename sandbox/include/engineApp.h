/** \file engineApp.cpp
*/
#pragma once

#include "include/independent/core/entrypoint.h"
#include "engine.h"
#include "UILayer.h"
#include "SceneLayer.h"
#include "AssimpTestLayer.h"
#include "EnTTLayer.h"
#include "FramebufferLayer.h"
/**
\class EngineApp
* \brief entry class to run the application
*/

class engineApp : public Engine::Application
{
public:
	engineApp();	//!< constructor
	~engineApp() override; //!< destructor

private:
	std::shared_ptr<UILayer> m_uiLayer;
	std::shared_ptr<SceneLayer> m_sceneLayer;
};
