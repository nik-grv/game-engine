/** \file TextureAtlas.cpp*/
#include <engine_pch.h>
#include "rendering\TextureAtlas.h"
#include <stb_image.h>

namespace Engine
{
	TextureAtlas::TextureAtlas(glm::ivec2 size, uint32_t channels, uint32_t reservedSpaces)
	{
		m_baseTex.reset(TextureRend::create(size.x, size.y, channels, nullptr)); 
		
		m_spaces.reserve(reservedSpaces);
		m_spaces.push_back({ 0,0,size.x,size.y});

	}

	bool TextureAtlas::add(const char* filePath, SubTexture& result)
	{
		int32_t width, height, channels;
		unsigned char* data = stbi_load(filePath, &width, &height, &channels,static_cast<int>(getChannels()));

		if (data)
		{
			return add(width, height, channels, data, result);
		}

		stbi_image_free(data);
		return false;
	}
	bool TextureAtlas::add(int32_t width, int32_t height, uint32_t channels, unsigned char* data, SubTexture& result)
	{
		//exit if channels	dont match
		if (channels != getChannels())
			return false;

		if (width == 0 || height == 0)
		{
			result = SubTexture(m_baseTex, glm::vec2(0.0f), glm::vec2(0.0f));
			return true;
		}

		for (auto it = m_spaces.begin();it != m_spaces.end();++it)
		{
			SimpleRect& space = *it;
			//check if the texture fits the space
			if (width < space.width && height < space.height)
			{
				//texture fits then add texture data
				m_baseTex->edit(space.x, space.y, width, height, data);

				//set subtexture
				glm::vec2 uvStart(static_cast<float>(space.x)/m_baseTex->getWidthf(), static_cast<float>(space.y) / m_baseTex->getHeightf());
				glm::vec2 uvEnd(static_cast<float>(space.x + width) / m_baseTex->getWidthf(), static_cast<float>(space.y + height) / m_baseTex->getHeightf());
				result = SubTexture(m_baseTex, uvStart, uvEnd);

				//find the remaining spaces
				//case 1 = if texture widht and height matches the space
				if (width == space.width && height == space.height)
				{
					m_spaces.erase(it);
					return true;
				}
				//case 2 - if width matches, height does not match-> split horizontally
				else if (width == space.width)
				{
					space.y += height;
					space.height -= height;
					return true;
				}
				//case 3 - if height matches, width does not match-> split vertically
				else if (height == space.height)
				{
					space.x += width;
					space.width-= width;
					return true;
				}
				//case 4 - neither width or height matches, -> split vertically and horizontally
				else
				{
					SimpleRect tmpRect = { space.x,space.y + height,width,space.height - height };
					space.x += width;
					space.width-= width;
					m_spaces.push_back(tmpRect);

					std::sort(m_spaces.begin(), m_spaces.end(), [](SimpleRect& a, SimpleRect& b)
					{return a.width < b.width; }
					);

					return true;
				}
			}
		}
		return false;
	}
}