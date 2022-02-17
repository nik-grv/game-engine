/** \file OpenGLFramebuffer.cpp*/

#include "engine_pch.h"
#include <glad/glad.h>
#include "platform/OpenGL/OpenGLFramebuffer.h"
#include "systems/log.h"
#include "rendering/renderBuffer.h"

namespace Engine
{
	OpenGLFramebuffer::OpenGLFramebuffer(glm::ivec2 size, FramebufferLayout layout)
	{
		m_size = size;
		m_layout = layout;

		glGenFramebuffers(1, &m_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		uint32_t colAttachmentCount = 0;
		for (auto& [type, isSampled] : m_layout)
		{
			if (isSampled)
			{
				switch (type)
				{
				case AttachmentType::Color:
					m_sampleTargets.push_back(std::shared_ptr<TextureRend>(TextureRend::create(m_size.x, m_size.y, 3, nullptr)));
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colAttachmentCount, GL_TEXTURE_2D, m_sampleTargets.back()->getRenderID(), 0);
					colAttachmentCount++;
					break;
				case AttachmentType::Depth:
					m_sampleTargets.push_back(std::shared_ptr<TextureRend>(TextureRend::create(m_size.x, m_size.y, 2, nullptr)));
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_sampleTargets.back()->getRenderID(), 0);
					break;
				}
			}
			else
			{
				switch (type) {
				case AttachmentType::Depth:
					m_nonSampledTargets.push_back(std::shared_ptr<RenderBuffer>(RenderBuffer::create(type, m_size)));
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_nonSampledTargets.back()->GetID());
					break;
				case AttachmentType::DepthAndStencil:
					m_nonSampledTargets.push_back(std::shared_ptr<RenderBuffer>(RenderBuffer::create(type, m_size)));
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_nonSampledTargets.back()->GetID());
					break;
				}
			}

		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			Log::error("Framebuffer not completed...");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		m_sampleTargets.clear();
		glDeleteFramebuffers(1, &m_id);
	}

	void OpenGLFramebuffer::use()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	}

	std::shared_ptr<TextureRend> OpenGLFramebuffer::getTexture(uint32_t index)
	{
		std::shared_ptr<TextureRend> result = nullptr;

		if (index < m_sampleTargets.size())
			result = m_sampleTargets[index];

		return result;
	}
}