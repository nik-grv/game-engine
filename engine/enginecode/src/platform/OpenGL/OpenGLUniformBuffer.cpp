/** \file OpenGLUniformBuffer.cpp*/

#include "engine_pch.h"
#include "platform/OpenGL/OpenGLUniformBuffer.h"
#include <glad/glad.h>

namespace Engine
{
	uint32_t OpenGLUniformBuffer::s_blockNumber = 0;
	
	UniformBufferLayout::BufferLayout()
	{

	}

	OpenGLUniformBuffer::OpenGLUniformBuffer(const UniformBufferLayout& layout)
	{
		m_ubLayout = layout;
		m_blockNumber = s_blockNumber;
		s_blockNumber++;
		glGenBuffers(1, &m_OpenGL_ID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_OpenGL_ID);
		glBufferData(GL_UNIFORM_BUFFER, m_ubLayout.getStride(), nullptr, GL_DYNAMIC_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, m_blockNumber, m_OpenGL_ID, 0, m_ubLayout.getStride());


		for (auto& element : layout)
		{
			m_uniformCache[element.m_name] = std::pair<uint32_t, uint32_t>(element.m_offset, element.m_size);
		}
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_OpenGL_ID);
	}

	void OpenGLUniformBuffer::attachShaderBlock(const std::shared_ptr<ShaderRend>& shader, const char* blockName)
	{
		uint32_t blockIndex = glGetUniformBlockIndex(shader->getRenderID(), blockName);
		glUniformBlockBinding(shader->getRenderID(), blockIndex, m_blockNumber);

	}

	void OpenGLUniformBuffer::uploadDataToUB(const char* uniformName, void* data)
	{
		auto& pair = m_uniformCache[uniformName];
		glBufferSubData(GL_UNIFORM_BUFFER, pair.first, pair.second, data);

	}
}