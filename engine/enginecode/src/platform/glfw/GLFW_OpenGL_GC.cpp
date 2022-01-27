/** \file GLFW_OpenGL_GC.cpp
*/

#include "engine_pch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "platform/GLFW/GLFW_OpenGL_GC.h"
#include "systems/log.h"

namespace Engine
{
	void GLFW_OpenGL_GC::init()
	{
		glfwMakeContextCurrent(m_window);
		auto result = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		if (!result)
		{
			Log::error("Could not create OPEN GL Context for current GLFW Window {0}", result);
		}

		//OpenGL Error Callback
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(
		[]
		(
			GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam
			)
		{
			switch (source)
			{
			case GL_DEBUG_SOURCE_API:
				Log::info(source); 
				break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
				Log::info(source);
				break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER:
				Log::info(source);
				break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:
				Log::info(source);
				break;
			case GL_DEBUG_SOURCE_APPLICATION:
				Log::info(source);
				break; 
			case GL_DEBUG_SOURCE_OTHER:
				Log::info(source);
				break; 
			}

			switch (type)
			{
			case GL_DEBUG_TYPE_ERROR:
				Log::info(type);
				break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				Log::info(type);
				break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				Log::info(type);
				break;
			case GL_DEBUG_TYPE_PORTABILITY:
				Log::info(type);
				break;
			case GL_DEBUG_TYPE_PERFORMANCE:
				Log::info(type);
				break;
			case GL_DEBUG_TYPE_MARKER:
				Log::info(type);
				break;
			case GL_DEBUG_TYPE_PUSH_GROUP:
				Log::info(type);
				break;
			case GL_DEBUG_TYPE_POP_GROUP:
				Log::info(type);
				break;
			case GL_DEBUG_TYPE_OTHER:
				Log::info(type);
				break;
			}

			switch (severity)
			{
			case GL_DEBUG_SEVERITY_LOW:
				Log::warn(message);
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				Log::trace(message);
				break;
			case GL_DEBUG_SEVERITY_HIGH:
				Log::error(message);
				break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				Log::warn(message);
				break;
			default:
				break;
			}
		}, nullptr);
	}

	void GLFW_OpenGL_GC::swapBuffer()
	{
		glfwSwapBuffers(m_window);
	}

}