/** \file OpenGLVertexBuffer.cpp*/

#include "engine_pch.h"
#include "platform/OpenGL/OpenGLVertexBuffer.h"
#include <glad/glad.h>

namespace Engine
{
	VertexBufferLayout::BufferLayout()
	{
	}
	

	OpenGLVertexBuffer::OpenGLVertexBuffer(void* verts, uint32_t size, VertexBufferLayout layout) : m_layout(layout)
	{
		glCreateBuffers(1, &m_OpenGL_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_OpenGL_ID);
		glBufferData(GL_ARRAY_BUFFER, size, verts, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_OpenGL_ID);
	}

	void OpenGLVertexBuffer::edit(void* verts, uint32_t size, uint32_t offset)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_OpenGL_ID);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, verts);
	}
}