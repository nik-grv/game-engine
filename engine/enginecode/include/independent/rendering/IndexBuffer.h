/** \file IndexBuffer.h
*/
#pragma once

#include <cstdint>

namespace Engine {

	/**
	\class IndexBuffer
	*\brief API Agnostic class to include OpenGLIndexBuffer
	*/
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;	//!< destructor
		virtual inline uint32_t getRenderID() const = 0; //!< get render iD
		virtual inline uint32_t getCount() const = 0; //!< get draw count

		//! Function to create an index buffer
		/*!
		\param indices uint32_t - indices in the object
		\param count uint32_t - vertices count
		*/
		static IndexBuffer* create(uint32_t* indices, uint32_t count);
	};
}