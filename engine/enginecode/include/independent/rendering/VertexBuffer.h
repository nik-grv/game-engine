/** \file VertexBuffer.h
*/
#pragma once

#include <cstdint>
#include "rendering/bufferLayout.h"

namespace Engine {

	/**
	\class VertexBuffer
	*\brief API Agnostic class to include OpenGLVertexBuffer
	*/
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;	//!< destructor
		virtual inline uint32_t getRenderID() const = 0; //!< get render iD
		virtual inline const VertexBufferLayout& getLayout() const = 0; //!< get buffer layout
		virtual inline void edit(void* verts, uint32_t size, uint32_t offset) = 0; //!<  edit the VBO
		static VertexBuffer* create(void* verts, uint32_t size, const VertexBufferLayout& layout); //!<  static function to create in API agnostic code
	};
}