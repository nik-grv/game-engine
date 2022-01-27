/** \file OpenGLVertexArray.h*/
#pragma once

#include <vector>
#include <memory>

#include "rendering/VertexArray.h"

namespace Engine {

	/**
	\class OpenGLVertexArray
	*\brief class to hold vertex array data. Inherits from VertexArray
	*/
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray(); //!< constructor
		virtual ~OpenGLVertexArray(); //!< destructor
		virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)override; //!< add vertex buffer
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)override; //!< set the index buffer
		virtual inline uint32_t getRenderID() const override { return m_OpenGL_ID; } //!< get render iD
		virtual inline std::vector<std::shared_ptr<VertexBuffer>> getVertexBuffers() const override { return m_vertexBuffer; } //!< get vertex buffers
		virtual inline std::shared_ptr<IndexBuffer> getIndexBuffers() const override { return m_indexBuffer; } //!< get index buffers
		virtual inline uint32_t getDrawCount() const override//!< get draw count for the index buffer 
		{
			if (m_indexBuffer)
				return m_indexBuffer->getCount();
			else
				return 0;
		}
	private:
		uint32_t m_OpenGL_ID; //!< render ID
		uint32_t m_attribIndex = 0; //!< attribute index 
		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffer; //!< vector of vertex buffer pointer
		std::shared_ptr<IndexBuffer> m_indexBuffer; //!< pointer to index buffer
	};

}