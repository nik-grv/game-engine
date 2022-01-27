/** \file subTexture.h*/
#pragma once

#include "platform/OpenGL/OpenGLTexture.h"
#include <memory>
#include <glm/glm.hpp>
namespace Engine {

	/**
	\class SubTexture
	*\brief abstraction for loading texture
	*/
	class SubTexture
	{
	public:
		SubTexture() {}; //!< constructor
		SubTexture(const std::shared_ptr<TextureRend>& texture, const glm::vec2& uvStart, const glm::vec2& uvEnd); //!< constructor with pointer to OpenGLTexture and uv start end
		inline glm::vec2 getUVStart() const { return m_uvStart; } //!< get uv start points
		inline glm::vec2 getUVEnd() const { return m_uvEnd; } //!< get uv end points
		glm::ivec2 getSize() const { return m_size; } //!< get size in int
		glm::vec2 getSizef() const { return { static_cast<float>(m_size.x),static_cast<float>(m_size.y) }; } //!< get size in float
		inline uint32_t getWidth() const { return m_size.x; } //!< get width of texture
		inline uint32_t getHeight()const { return m_size.y; } //!< get height of texture
		inline uint32_t getWidthf() const { return static_cast<float>(m_size.x); } //!< get width of texture
		inline uint32_t getHeightf() const { return static_cast<float>(m_size.y); } //!< get height of texture
		float transformU(float u); //!< calculate U atlas coordinate 
		float transformV(float v); //!< calculate V atlas coordinate
		glm::vec2 transformUV(glm::vec2 uv); //!< transform to atlas cooridnate by using vec2 instead of float
		inline const std::shared_ptr<TextureRend>& getBaseTexture() const { return m_texture; } //!< get base texture
	private:
		std::shared_ptr<TextureRend> m_texture; //!< member pointer to OpenGLtexture
		glm::vec2 m_uvStart;	//!< uv start points
		glm::vec2 m_uvEnd;		//!< uv end points
		glm::ivec2 m_size;		//!< size of texture
		float m_uvWidth;		//!< width of uv in atlas
		float m_uvHeight;		//!< height of uv in atlas
	};
}