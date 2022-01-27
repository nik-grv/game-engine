/** \file VertexArray.h
*/
#pragma once

#include <cstdint>
#include "rendering/VertexBuffer.h"
#include "rendering/IndexBuffer.h"

namespace Engine {

	/**
	\class VertexArray
	*\brief API Agnostic class to include OpenGLVertexArray
	*/
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;	//!< destructor
		virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0; //!< add vertex buffer
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0; //!< set the index buffer
		virtual inline uint32_t getRenderID() const = 0; //!< get render iD
		virtual inline uint32_t getDrawCount() const = 0; //!< get draw count for the index buffer 
		virtual inline std::vector<std::shared_ptr<VertexBuffer>> getVertexBuffers() const = 0; //!< get vertex buffers
		virtual inline std::shared_ptr<IndexBuffer> getIndexBuffers() const = 0; //!< get index buffer
		static VertexArray* create();
	};
}