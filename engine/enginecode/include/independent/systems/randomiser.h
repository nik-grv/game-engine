#pragma once

#include <random>
#include "system.h"
#include <memory>

namespace Engine {
	class Randomiser : public System {
	public:
		virtual void start(SystemSignal init = SystemSignal::None, ...) override; //!< Start the system
		virtual void stop(SystemSignal close = SystemSignal::None, ...) override; //!< Stop the system

		static uint32_t uniformIntBetween(int32_t lower, int32_t upper);
		static float uniformFloatBetween(float lower, float upper);
		static int32_t normalInt(float c, float sigma);
		static int32_t normalFloat(float c, float sigma);

	private:
		static std::shared_ptr<std::mt19937> s_generator;
		static std::uniform_int_distribution<int32_t> s_uniformInt;
		static std::uniform_real_distribution<float> s_uniformFloat;
		static float s_intRange;
		static float s_fRange;
	};
}