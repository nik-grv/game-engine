/** \file OpenGLUniformBuffer.h*/
#pragma once

#include "rendering/UniformBuffer.h"

namespace Engine {

	/**
	\class OpenGLUniformBuffer
	*\brief class to hold uniform buffer data. Inherits from UniformBuffer
	*/
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(const UniformBufferLayout& layout); //!< constructor
		~OpenGLUniformBuffer(); //!< destructor
		inline uint32_t getRenderID() override { return m_OpenGL_ID; } //!< get render iD
		inline UniformBufferLayout getLayout() override { return m_ubLayout; } //!< get uniform buffer layout
		void attachShaderBlock(const std::shared_ptr<ShaderRend>& shader, const char* blockName); //!< attach shader
		void uploadDataToUB(const char* uniformName, void* data) override; //!< upload data to uniform buffer
	private:
		uint32_t m_OpenGL_ID; //!< render id
		static uint32_t s_blockNumber; //!< global block number
	};
}