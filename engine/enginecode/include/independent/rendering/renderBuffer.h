#pragma once
/** \file RenderBuffer.h
*/

#include "TextureRend.h"
<<<<<<< HEAD
#include <glm/glm.hpp>

namespace Engine {
	enum class AttachmentType
	{
		Color, Depth, Stencil, DepthAndStencil
	};
=======
#include "framebuffer.h"

namespace Engine {
>>>>>>> TG-45-frame-buffers

	class RenderBuffer {
	public:
		virtual ~RenderBuffer() = default;
		virtual uint32_t GetID() = 0;

		static RenderBuffer* create(AttachmentType type, glm::ivec2 size);
<<<<<<< HEAD

=======
>>>>>>> TG-45-frame-buffers
	};
}