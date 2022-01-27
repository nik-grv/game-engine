/** \file graphicsContext.h
*/
#pragma once

namespace Engine
{
	/**
	\class GraphicsContext
	*\brief virtual class to provide interface for OpenGL graphics context 
	*/
	class GraphicsContext
	{
	public:
		virtual void init() = 0; //!< init	the graphics context for current windowing API
		virtual void swapBuffer() = 0; //!< swap the front and back buffers (double buffering)
	};
}