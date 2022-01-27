#include <engine_pch.h>
#include "rendering/TextureUnitManager.h"

namespace Engine
{
	TextureUnitManager::TextureUnitManager()
	{
	}
	void TextureUnitManager::clear()
	{
		m_head = 0;
		m_tail = 0;
		m_isFull = false;
		std::fill(m_buffer.begin(), m_buffer.end(), 0xFFFFFFFF);
	}

	bool TextureUnitManager::getUnit(uint32_t texID, uint32_t& textureUnit)
	{
		//check if the texture is already bound
		for (int i = m_tail; i < m_head; i++)
		{
			if (m_buffer.at(i) == texID)
			{
				textureUnit = i;
				return false;
			}
		}
		//check if space is in buffer..
		if (m_isFull) clear();

		//put texture id in buffer
		m_buffer.at(m_head) = texID;
		textureUnit = m_head;

		//check if the buffer is full
		m_head = (++m_head) % m_capacity;
		if (m_head == m_tail)
			m_isFull = true;

		return true;
	}
}