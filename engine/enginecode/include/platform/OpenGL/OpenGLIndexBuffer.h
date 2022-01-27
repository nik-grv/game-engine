/** \file OpenGLIndexBuffer.h
*/
#pragma once

#include "rendering/IndexBuffer.h"
namespace Engine {

	/**
	\class OpenGLIndexBuffer
	*\brief class to hold index buffer data. Inherits from IndexBuffer
	*/
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:

		//! Overloaded constructor for index buffer
		/*!
		\param indices uint32_t - indices of object
		\param count uint32_t - count of vertices
		*/
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer(); //!< destructor
		virtual inline uint32_t getRenderID() const override{ return m_OpenGL_ID; } //!< get render iD
		virtual inline uint32_t getCount() const override{ return m_count; } //!< get draw count
	private:
		uint32_t m_OpenGL_ID; //!< render ID
		uint32_t m_count; //!< draw count
	};
}