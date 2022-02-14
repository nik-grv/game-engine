/** \file RenderAPI.cpp*/

#include "engine_pch.h"
#include "systems/log.h"
#include "rendering/RenderAPI.h"

#include "platform/OpenGL/OpenGLIndexBuffer.h"
#include "platform/OpenGL/OpenGLVertexBuffer.h"
#include "platform/OpenGL/OpenGLVertexArray.h"
#include "platform/OpenGL/OpenGLTexture.h"
#include "platform/OpenGL/OpenGLShader.h"
#include "platform/OpenGL/OpenGLUniformBuffer.h"
#include "platform/OpenGL/OpenGLFramebuffer.h"
#include "platform/OpenGL/OpenGLRenderBuffer.h"


namespace Engine
{
	RenderAPI::API RenderAPI::s_API = RenderAPI::API::OpenGL;

	IndexBuffer* IndexBuffer::create(uint32_t* indices, uint32_t count)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::info("No rendering API");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLIndexBuffer(indices, count);
			break;
		case RenderAPI::API::Direct3d:
			Log::info("Direcr3D not supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::info("Vulkan not supported");
			break;
		default:
			break;
		}
		return nullptr;
	}

	VertexBuffer* VertexBuffer::create(void* verts, uint32_t size, const VertexBufferLayout& layout)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::info("No rendering API");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLVertexBuffer(verts,size,layout);
			break;
		case RenderAPI::API::Direct3d:
			Log::info("Direcr3D not supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::info("Vulkan not supported");
			break;
		default:
			break;
		}
		return nullptr;
	}

	/*void VertexBuffer::edit(void* verts, uint32_t size, uint32_t offset)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::info("No rendering API");
			break;
		case RenderAPI::API::OpenGL:
			const VertexBufferLayout& layout = VertexBufferLayout({});
			OpenGLVertexBuffer::create(verts, size,layout);
			break;
		case RenderAPI::API::Direct3d:
			Log::info("Direcr3D not supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::info("Vulkan not supported");
			break;
		default:
			break;
		}
	}*/

	VertexArray* VertexArray::create()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::info("No rendering API");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLVertexArray();
			break;
		case RenderAPI::API::Direct3d:
			Log::info("Direcr3D not supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::info("Vulkan not supported");
			break;
		default:
			break;
		}
		return nullptr;
	}

	TextureRend* TextureRend::create(const char* filePath)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::info("No rendering API");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLTexture(filePath);
			break;
		case RenderAPI::API::Direct3d:
			Log::info("Direcr3D not supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::info("Vulkan not supported");
			break;
		default:
			break;
		}
		return nullptr;
	}
	
	TextureRend* TextureRend::create(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::info("No rendering API");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLTexture(width,height,channels,data);
			break;
		case RenderAPI::API::Direct3d:
			Log::info("Direcr3D not supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::info("Vulkan not supported");
			break;
		default:
			break;
		}
		return nullptr;
	}
	
	ShaderRend* ShaderRend::create(const char* filePath)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::info("No rendering API");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLShader(filePath);
			break;
		case RenderAPI::API::Direct3d:
			Log::info("Direcr3D not supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::info("Vulkan not supported");
			break;
		default:
			break;
		}
		return nullptr;
	}
	
	ShaderRend* ShaderRend::create(const char* vertexFilePath, const char* fragmentFilePath)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::info("No rendering API");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLShader(vertexFilePath, fragmentFilePath);
			break;
		case RenderAPI::API::Direct3d:
			Log::info("Direcr3D not supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::info("Vulkan not supported");
			break;
		default:
			break;
		}
		return nullptr;
	}
	
	UniformBuffer* UniformBuffer::create(const UniformBufferLayout& layout)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::info("No rendering API");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLUniformBuffer(layout);
			break;
		case RenderAPI::API::Direct3d:
			Log::info("Direcr3D not supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::info("Vulkan not supported");
			break;
		default:
			break;
		}
		return nullptr;
	}
	
	
	Framebuffer* Framebuffer::createDefault()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::info("No rendering API");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLFramebuffer();
			break;
		case RenderAPI::API::Direct3d:
			Log::info("Direcr3D not supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::info("Vulkan not supported");
			break;
		default:
			break;
		}
		return nullptr;
	}
	
	Framebuffer* Framebuffer::create(glm::ivec2 size, FramebufferLayout layout)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::info("No rendering API");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLFramebuffer(size,layout);
			break;
		case RenderAPI::API::Direct3d:
			Log::info("Direcr3D not supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::info("Vulkan not supported");
			break;
		default:
			break;
		}
		return nullptr;
	}

	RenderBuffer* RenderBuffer::create(AttachmentType type, glm::ivec2 size)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::info("No rendering API");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLRenderBuffer(type, size);
			break;
		case RenderAPI::API::Direct3d:
			Log::info("Direcr3D not supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::info("Vulkan not supported");
			break;
		default:
			break;
		}
		return nullptr;
	}

}