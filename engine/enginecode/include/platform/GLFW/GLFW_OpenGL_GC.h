/** \file GLFW_OpenGL_GC.h
*/
#pragma once

#include "core/graphicsContext.h"

namespace Engine
{
	/**
   \class GLFW_OpenGL_GC
   *\brief class to provide interface for the GLFW window context. Inherits from GraphicsContext
   */
	class GLFW_OpenGL_GC : public GraphicsContext
	{
	public:
		GLFW_OpenGL_GC(GLFWwindow* win) : m_window(win){} //!< constructor to set the window ref
		virtual void init() override; //!< init	the graphics context for current windowing API
		virtual void swapBuffer() override; //!< swap the front and back buffers (double buffering)
	private:
		GLFWwindow* m_window;//!<pointer to glfw window

	};
}