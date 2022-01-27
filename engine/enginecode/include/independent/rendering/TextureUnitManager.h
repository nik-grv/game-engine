/** \file TextureUnitManager.h*/
#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
namespace Engine {

	/**
	\class TextureUnitManager
	* \brief class to store texture in a ring buffer
	*/
	class TextureUnitManager
	{
	public:
		TextureUnitManager(); //!< default constructor
		TextureUnitManager(uint32_t capacity) : m_capacity(capacity), m_buffer(capacity, 0xFFFFFFFF) {} //!< constructor which inits the capacity
		inline bool full() { return m_isFull; } //!< is the buffer full
		void clear(); //!< clear the buffer
		bool getUnit(uint32_t texID, uint32_t& textureUnit); //!<return whether or not the texture needs binding. Texture unit is bound to the unit
	private:
		uint32_t m_capacity; //!< capacity of ring buffer
		std::vector<uint32_t> m_buffer; //!< vector to store the buffer
		bool m_isFull = false; //!< is the buffer full
		uint32_t m_head = 0; //!< head of the buffer
		uint32_t m_tail = 0; //!< tail of the buffer
	};

}