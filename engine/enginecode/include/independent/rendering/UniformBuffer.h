/** \file UniformBuffer.h*/
#pragma once

#include "bufferLayout.h"
#include "ShaderRend.h"
#include <unordered_map>
#include <memory>

namespace Engine {

	/**
	\class UniformBuffer
	*\brief API Agnostic class to include OpenGLUniformBuffer
	*/
	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default; //!< destructor
		virtual inline uint32_t getRenderID() = 0; //!< get render iD
		virtual inline UniformBufferLayout getLayout() = 0;//!< get uniform buffer layout
		virtual void attachShaderBlock(const std::shared_ptr<ShaderRend>& shader, const char* blockName) = 0; //!< attach shader
		virtual void uploadDataToUB(const char* uniformName, void* data) = 0; //!< upload data to uniform buffer
		static UniformBuffer* create(const UniformBufferLayout& layout); //!<  static function to create in API agnostic code
	protected:
		UniformBufferLayout m_ubLayout; //!< uniform buffer layout object
		std::unordered_map<const char*, std::pair<uint32_t, uint32_t>> m_uniformCache; //!< store uniform name with offset and size values
		uint32_t m_blockNumber; //!< block number of this UBO
	};
}