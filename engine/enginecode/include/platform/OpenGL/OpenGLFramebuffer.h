/** \file OpenGLFramebuffer.h
*/
#pragma once

#include "rendering/Framebuffer.h"

namespace Engine 
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer() { m_id = 0; }
		OpenGLFramebuffer(glm::ivec2 size, FramebufferLayout layout);
		~OpenGLFramebuffer() override;
		void OnResize(WindowResizeEvent& e) override {};
		void use() override;
		uint32_t getRenderID() override { return m_id; }
		std::shared_ptr<TextureRend> getTexture(uint32_t index) override; // get render texture at index
	private:
		uint32_t m_id;
	};


}