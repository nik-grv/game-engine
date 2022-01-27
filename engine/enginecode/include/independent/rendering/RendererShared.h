/** \file RendererShared.h*/
#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>
#include "rendering/shaderDataType.h"
#include "rendering/TextureRend.h"
#include "rendering/subTexture.h"
#include "rendering/ShaderRend.h"
#include "rendering/RenderAPI.h"
#include "rendering/VertexArray.h"
#include "rendering/UniformBuffer.h"
#include "rendering/RendererCommands.h"
#include "rendering/TextureUnitManager.h"
#include "rendering/TextureAtlas.h"

namespace Engine 
{
	using SceneWideUniforms = std::unordered_map<const char*, std::pair<ShaderDataType, void*>>; //!< sceen wide uniform 

	/**
	\class RendererShared
	* \brief shared class with common renderer functions and variables
	*/
	class RendererShared
	{
	public:

		//! Function to execute a render commands
		/*!
		\param command RendererCommands - renderer command
		*/
		static void actionCommand(std::shared_ptr<RendererCommands>& command) { command->m_action(); }
		static TextureUnitManager s_tum; //!< texture unit manager
		static const unsigned int SCR_WIDTH = 1280; //!< screen width
		static const unsigned int SCR_HEIGHT = 720; //!< screen height

	};

}