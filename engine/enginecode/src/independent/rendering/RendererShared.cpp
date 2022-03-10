#include "engine_pch.h"
#include "rendering/RendererShared.h"


namespace Engine {

	uint32_t RendererShared::pack(const glm::vec4& tint)
	{
		uint32_t result = 0;

		uint32_t r = (static_cast<uint32_t>(tint.r * 225.f)) << 0;
		uint32_t g = (static_cast<uint32_t>(tint.g * 225.f)) << 8;
		uint32_t b = (static_cast<uint32_t>(tint.b * 225.f)) << 16;
		uint32_t a = (static_cast<uint32_t>(tint.a * 225.f)) << 24;

		result = (r | g | b | a);
		return result;
	}
}