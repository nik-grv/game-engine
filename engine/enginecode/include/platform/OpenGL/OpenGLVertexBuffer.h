/** \file OpenGLVertexBuffer.h
*/
#pragma once

#include "rendering/bufferLayout.h"
#include "rendering/VertexBuffer.h"

namespace Engine {

	/**
	\class OpenGLVertexBuffer
	*\brief class to hold vertex buffer data. Inherits from VertexBuffer
	*/
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(void* verts, uint32_t size, VertexBufferLayout layout); //!< constructor
		~OpenGLVertexBuffer(); //!< destructor
		virtual void edit(void* verts, uint32_t size, uint32_t offset); //!< edit vertex buffer data
		virtual inline uint32_t getRenderID() const override{ return m_OpenGL_ID; } //!< get render iD
		virtual inline const VertexBufferLayout& getLayout() const override{ return m_layout; } //!< get buffer layout
	private:
		uint32_t m_OpenGL_ID; //!< render ID
		VertexBufferLayout m_layout;
	};
}