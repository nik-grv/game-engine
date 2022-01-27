/** \file subTexture.cpp*/

#include "engine_pch.h"
#include "rendering/subTexture.h"

namespace Engine 
{
	SubTexture::SubTexture(const std::shared_ptr<TextureRend>& texture, const glm::vec2& uvStart, const glm::vec2& uvEnd) :
		m_texture(texture),m_uvStart(uvStart),m_uvEnd(uvEnd)
	{
		m_uvWidth = m_uvEnd.x - m_uvStart.x;
		m_uvHeight = m_uvEnd.y - m_uvStart.y;
		m_size.x = static_cast<int>((m_uvEnd.x - m_uvStart.x) * m_texture->getWidthf());
		m_size.y = static_cast<int>((m_uvEnd.y - m_uvStart.y) * m_texture->getHeightf());

	}

	float SubTexture::transformU(float u)
	{
		return m_uvStart.x + ((m_uvEnd.x - m_uvStart.x) * u);
	}
	float SubTexture::transformV(float v)
	{
		return m_uvStart.y + ((m_uvEnd.y - m_uvStart.y) * v);
	}
	glm::vec2 SubTexture::transformUV(glm::vec2 uv)
	{
		return m_uvStart + ((m_uvEnd - m_uvStart) * uv);
	}
}