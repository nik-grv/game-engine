#include "engine_pch.h"
#include <glad/glad.h>
#include "platform/OpenGL/OpenGLRenderBuffer.h"
#include "systems/log.h"

namespace Engine {

	OpenGLRenderBuffer::OpenGLRenderBuffer(AttachmentType type, glm::ivec2 size)
	{
		glGenRenderbuffers(1, &m_ID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_ID);

		switch (type) {
		case AttachmentType::Depth: glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y); break;
		case AttachmentType::DepthAndStencil: glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y); break;
		}
	}

	OpenGLRenderBuffer::~OpenGLRenderBuffer()
	{

	}
}

