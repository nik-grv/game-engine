/** \file OpenGLRenderBuffer.h
*/
#pragma once
#include "rendering/renderBuffer.h"


namespace Engine {
	class OpenGLRenderBuffer : public RenderBuffer 
	{
	public:
		OpenGLRenderBuffer() { m_ID = 0; }
		OpenGLRenderBuffer(AttachmentType type, glm::ivec2 size);
		~OpenGLRenderBuffer() override;
		uint32_t GetID() override { return m_ID; }
	private:
		uint32_t m_ID;
	};
}