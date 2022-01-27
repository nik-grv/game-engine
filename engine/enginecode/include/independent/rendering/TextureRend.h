/** \file TextureRend.h
*/
#pragma once

namespace Engine {

	/**
	\class TextureRend
	*\brief API Agnostic class to include OpenGLTexture
	*/
	class TextureRend
	{
	public:
		virtual ~TextureRend() = default; //!< destructor
		virtual void edit(uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height, unsigned char* data) = 0; //!< edit texture
		virtual inline uint32_t getRenderID()const = 0; //!< get render iD
		virtual inline uint32_t getWidth() const = 0;//!< get width of texture
		virtual inline uint32_t getHeight() const = 0;//!< get height of texture
		virtual inline uint32_t getWidthf() const = 0; //!< get width of texture
		virtual inline uint32_t getHeightf() const = 0; //!< get height of texture
		virtual inline uint32_t getChannels() const = 0; //!< get channels of texture
		virtual void init(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data) = 0;	//!< setup data in this to be used by the 2 constructors

		static TextureRend* create(const char* filePath); //!< static pointer to take file path
		static TextureRend* create(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data); //!< static pointer to take texture properties
	};
}