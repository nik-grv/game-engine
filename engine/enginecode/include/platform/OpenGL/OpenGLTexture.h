/** \file OpenGLTexture.h
*/
#pragma once

#include <cstdint>
#include "rendering/TextureRend.h"

namespace Engine {

	/**
	\class OpenGLTexture
	*\brief abstraction for loading texture. Inherits from TextureRend
	*/
	class OpenGLTexture : public TextureRend
	{
	public:
		OpenGLTexture(const char* filePath); //!< constructor to take file path
		OpenGLTexture(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data); //!< constructor to take texture properties
		virtual ~OpenGLTexture(); //!< destructor
		virtual void edit(uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height, unsigned char* data) override; //!< edit texture
		virtual inline uint32_t getRenderID()const override { return m_OpenGL_ID; } //!< get render iD
		virtual inline uint32_t getWidth() const override { return m_width; } //!< get width of texture
		virtual inline uint32_t getHeight() const override{ return m_height; } //!< get height of texture
		virtual inline uint32_t getWidthf() const override{ return static_cast<float>(m_width); } //!< get width of texture
		virtual inline uint32_t getHeightf() const override{ return static_cast<float>(m_height); } //!< get height of texture
		virtual inline uint32_t getChannels() const override{ return m_channels; } //!< get channels of texture
	private:
		unsigned int m_OpenGL_ID; //!< render id
		uint32_t m_width;		//<! width of texture
		uint32_t m_height;		//<! height of texture
		uint32_t m_channels;	//<! channels of texture
		virtual void init(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data);			//!< setup data in this to be used by the 2 constructors
	};
}