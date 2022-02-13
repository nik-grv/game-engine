#pragma once
/** \file RenderBuffer.h
*/

#include "TextureRend.h"
#include "framebuffer.h"

namespace Engine {

	class RenderBuffer {
	public:
		virtual ~RenderBuffer() = default;
		virtual uint32_t GetID() = 0;

		static RenderBuffer* create(AttachmentType type, glm::ivec2 size);
	};
}