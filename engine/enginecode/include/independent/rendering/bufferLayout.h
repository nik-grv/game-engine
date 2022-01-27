/** \file bufferLayout.h
*/
#pragma once

#include "shaderDataType.h"
#include <vector>

namespace Engine {

	/**
	\class VertexBufferElement
	*\brief class to hold data of a single element in opengl vertex buffer layout
	*/
	class VertexBufferElement
	{
	public:
		ShaderDataType m_dataType; //!< data type of element
		uint32_t m_size;		//!< size of element
		uint32_t m_offset;		//!< offset of element
		bool isNormalized;		//!< is normalized 

		VertexBufferElement() {} //!< default constructor

		//! Overloaded constructor to initialise the VertexBufferElement
		/*!
		\param shaderData ShaderDataType - The type of shader data
		\param normalised bool - is the data normalised
		*/
		VertexBufferElement(ShaderDataType type, bool normalized = false) :
			m_dataType(type), m_size(SDT::size(type)), m_offset(0), isNormalized(normalized) {}
	};


	/**
	\class UniformBufferElement
	*\brief class to hold data of a single element in opengl uniform buffer layout
	*/
	class UniformBufferElement
	{
	public:
		const char* m_name;
		ShaderDataType m_dataType; //!< data type of element
		uint32_t m_size;		//!< size of element
		uint32_t m_offset;		//!< offset of element
		
		UniformBufferElement() {} //!< default constructor

		//! Overloaded constructor to initialise the UniformBufferElement
		/*!
		\param name char - name of the uniform buffer data
		\param shaderData ShaderDataType - The type of shader data
		*/
		UniformBufferElement(const char* name, ShaderDataType type) :
			m_name(name), m_dataType(type), m_size(SDT::std140Alignment(type)), m_offset(0) {}//!< constructor to set the values
	};



	/**
	\class BufferLayout
	*\brief abstraction of templated buffer layout class
	*/
	template <class G>
	class BufferLayout
	{
	public:
		BufferLayout<G>(); //!< default constructor

		//! Templated classs function to initialise the buffer layout
		/*!
		\param element list - vector list of elements
		*/
		BufferLayout<G>(const std::initializer_list<G>& element) :m_elements(element) {calculateStrideAndOffset();}//!< constructor with the buffer element list
		inline uint32_t getStride() const { return m_stride; } //!< get stride

		//! Function to add element to the list
		/*!
		\param element G - templated element type
		*/
		void addElement(G element); //!< add element
		inline typename std::vector<G>::iterator begin() { return m_elements.begin(); } //!< iterator to iterate through all the elements in the vector
		inline typename std::vector<G>::iterator end() { return m_elements.end(); } //!< iterator to iterate through all the elements in the vector
		inline typename std::vector<G>::const_iterator begin() const { return m_elements.begin(); } //!< iterator to iterate through all the elements in the vector
		inline typename std::vector<G>::const_iterator end() const { return m_elements.end(); } //!< iterator to iterate through all the elements in the vector

	private:
		std::vector<G> m_elements; //!< vector of buffer elements
		uint32_t m_stride;  //!< width in btyes of a buffer line
		void calculateStrideAndOffset(); //!< calc stride and offset based on the elements data
	};

	template <class G>
	void BufferLayout<G>::addElement(G element)
	{
		m_elements.push_back(element);
		calculateStrideAndOffset();
	}

	template <class G>
	void BufferLayout<G>::calculateStrideAndOffset()
	{
		uint32_t offset = 0;

		for (auto& element : m_elements)
		{
			element.m_offset = offset;
			offset += element.m_size;
		}

		m_stride = offset;
	}

	using VertexBufferLayout = BufferLayout<VertexBufferElement>;
	using UniformBufferLayout = BufferLayout<UniformBufferElement>;
}