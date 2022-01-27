/** \file TextureAtlas.h*/
#pragma once

#include "rendering/subTexture.h"
#include <algorithm>
#include <vector>

namespace Engine {
	
	/** \struct SimpleRect
	*\brief properties for a rectangle
	\param width int32_t - width of the window
	\param height int32_t - height of the window
	\param x int32_t - x position
	\param y int32_t - y position
	*/
	struct SimpleRect
	{
		int32_t x; //!< x position
		int32_t y;//!< y position
		int32_t width;//!< width of rectangle 
		int32_t height;//!< height of rectangle 
	};

	/**
	\class TextureAtlas
	* \brief class to store textures to be rendered
	*/
	class TextureAtlas
	{
	public: 
		TextureAtlas(glm::ivec2 size = glm::ivec2(4096, 4096), uint32_t channels = 4, uint32_t reservedSpaces= 32); //!< constructor with size and channels
		inline uint32_t getChannels() const { return m_baseTex->getChannels(); } //!< get channels of the texture
		inline std::shared_ptr<TextureRend> getBaseTexture() const { return m_baseTex; } //!< get base texture
		inline uint32_t getRenderID() { return m_baseTex->getRenderID(); } //!< get render id

		bool add(const char* filePath, SubTexture& result); //!< add sub texture from a file
		bool add(int32_t width, int32_t height, uint32_t channels, unsigned char* data, SubTexture& result); //!< add sub texture from dimension properties
	private:
		std::vector<SimpleRect> m_spaces; //!< spaces in the texture atlas
		std::shared_ptr<TextureRend> m_baseTex; //!< texture to hold subtexture pixel data
	};
}