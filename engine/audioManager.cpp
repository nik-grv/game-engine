/* \file audioManager.cpp*/
#include "engine_pch.h"

#include "audioManager.h"
#include "systems/log.h"

namespace Engine
{
	int32_t AudioManager::errorCheck(FMOD_RESULT result) const
	{
		if (result != FMOD_OK)
		{
			Log::error("FMOD ERROR: {0}", result);
			return 1;
		}
		return 0;
	}

	FMOD_VECTOR AudioManager::GLMVecToFmod(const glm::vec3& vec)
	{
		FMOD_VECTOR result;
		result.x = vec.x;
		result.y = vec.y;
		result.z = vec.z;
		return result;
	}

	void AudioManager::start(SystemSignal init, ...)
	{

	}
}