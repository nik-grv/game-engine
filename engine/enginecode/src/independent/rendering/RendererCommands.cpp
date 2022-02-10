/** \file RendererCommands.h*/

#include "engine_pch.h"
#include <glad/glad.h>
#include "rendering/RendererCommands.h"
#include "rendering/RenderAPI.h"
#include "systems/log.h"
#include "systems/log.h"

namespace Engine {

	std::function<void(void)> RenderCommandFactory::getClearColorAndDepthBuffer()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::info("No rendering API");
			return std::function<void(void)>();
		case RenderAPI::API::OpenGL:
			return []() {glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); };
		case RenderAPI::API::Direct3d:
			Log::info("Direcr3D not supported");
			return std::function<void(void)>();
		case RenderAPI::API::Vulkan:
			Log::info("Vulkan not supported");
			return std::function<void(void)>();
		default:
			return std::function<void(void)>();
		}
	}
	std::function<void(void)> RenderCommandFactory::getSetClearCommand(float r, float g, float b, float a)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::info("No rendering API");
			return std::function<void(void)>();
		case RenderAPI::API::OpenGL:
			return [r, g, b, a]() { glClearColor(r, g, b, a); };
		case RenderAPI::API::Direct3d:
			Log::info("Direcr3D not supported");
			return std::function<void(void)>();
		case RenderAPI::API::Vulkan:
			Log::info("Vulkan not supported");
			return std::function<void(void)>();
		default:
			return std::function<void(void)>();
		}
	}
	
	std::function<void(void)> RenderCommandFactory::glEnableCmd(GLenum cmd)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::info("No rendering API");
			return std::function<void(void)>();
		case RenderAPI::API::OpenGL:
			return [cmd]() { glEnable(cmd); };
		case RenderAPI::API::Direct3d:
			Log::info("Direcr3D not supported");
			return std::function<void(void)>();
		case RenderAPI::API::Vulkan:
			Log::info("Vulkan not supported");
			return std::function<void(void)>();
		default:
			return std::function<void(void)>();
		}
	}
	std::function<void(void)> RenderCommandFactory::glDisableCmd(GLenum cmd)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::info("No rendering API");
			return std::function<void(void)>();
		case RenderAPI::API::OpenGL:
			return [cmd]() { glDisable(cmd); };
		case RenderAPI::API::Direct3d:
			Log::info("Direcr3D not supported");
			return std::function<void(void)>();
		case RenderAPI::API::Vulkan:
			Log::info("Vulkan not supported");
			return std::function<void(void)>();
		default:
			return std::function<void(void)>();
		}
	}
	std::function<void(void)> RenderCommandFactory::glBlendFuncCmd(GLenum sFactor, GLenum dFactor)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::info("No rendering API");
			return std::function<void(void)>();
		case RenderAPI::API::OpenGL:
			return [sFactor , dFactor]() { glBlendFunc(sFactor,dFactor); };
		case RenderAPI::API::Direct3d:
			Log::info("Direcr3D not supported");
			return std::function<void(void)>();
		case RenderAPI::API::Vulkan:
			Log::info("Vulkan not supported");
			return std::function<void(void)>();
		default:
			return std::function<void(void)>();
		}
	}
	std::function<void(void)> RenderCommandFactory::glUseProgramCmd(GLuint program)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::info("No rendering API");
			return std::function<void(void)>();
		case RenderAPI::API::OpenGL:
			return [program]() { glUseProgram(program); };
		case RenderAPI::API::Direct3d:
			Log::info("Direcr3D not supported");
			return std::function<void(void)>();
		case RenderAPI::API::Vulkan:
			Log::info("Vulkan not supported");
			return std::function<void(void)>();
		default:
			return std::function<void(void)>();
		}
	}
	
	std::function<void(void)> RenderCommandFactory::setGlLine()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::info("No rendering API");
			return std::function<void(void)>();
		case RenderAPI::API::OpenGL:
			return []() { glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); };
		case RenderAPI::API::Direct3d:
			Log::info("Direcr3D not supported");
			return std::function<void(void)>();
		case RenderAPI::API::Vulkan:
			Log::info("Vulkan not supported");
			return std::function<void(void)>();
		default:
			return std::function<void(void)>();
		}
	}
	
	std::function<void(void)> RenderCommandFactory::setGlFill()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::info("No rendering API");
			return std::function<void(void)>();
		case RenderAPI::API::OpenGL:
			return []() { glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); };
		case RenderAPI::API::Direct3d:
			Log::info("Direcr3D not supported");
			return std::function<void(void)>();
		case RenderAPI::API::Vulkan:
			Log::info("Vulkan not supported");
			return std::function<void(void)>();
		default:
			return std::function<void(void)>();
		}
	}
}