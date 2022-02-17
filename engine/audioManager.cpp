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
		errorCheck(FMOD::Studio::System::create(&m_studioSystem));
		errorCheck(m_studioSystem->initialize(m_maxChannels, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, NULL));

		unsigned int version;
		errorCheck(FMOD::System_Create(&m_lowLevelSystem));
		errorCheck(m_lowLevelSystem->getVersion(&version));

		if (version < FMOD_VERSION)
		{
			Log::error("FMOD lib version {0} does not match header version {1}", version, FMOD_VERSION);

		}

		errorCheck(m_lowLevelSystem->init(m_maxChannels, FMOD_INIT_NORMAL, NULL));

		errorCheck(m_lowLevelSystem->set3DSettings(1.f, 1.f, 1.f));

		errorCheck(m_lowLevelSystem->setGeometrySettings(50.f));
	}

	void AudioManager::stop(SystemSignal close, ...)
	{
	}

	void AudioManager::update()
	{
	}

	void AudioManager::loadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS)
	{
	}

	void AudioManager::loadEvent(const std::string& strEventName)
	{
	}

	void AudioManager::loadSound(const std::string& strSoundName, bool b3d, bool bLooping, bool bStream, float minDist, float maxDist, RollOff rollOff)
	{
	}

	void AudioManager::unLoadSound(const std::string& strSoundName)
	{
	}

	void AudioManager::set3dListenerAndOrientation(const glm::mat4& transform, const glm::vec3& velocity)
	{
	}

	void AudioManager::addGeometry(const std::string& label, const AudioGeometryDefinition& def)
	{
	}

	void AudioManager::moveGeometry(const std::string& label, const glm::vec3& position, const glm::vec3& forward, const glm::vec3& up, const glm::vec3& scale)
	{
	}

	int32_t AudioManager::playSound(const std::string& strSoundName, const glm::vec3& vPos)
	{
		return int32_t();
	}

	void AudioManager::playEvent(const std::string& strEventName)
	{
	}

	void AudioManager::toggleChannelPause(int32_t nChannelId)
	{
	}

	void AudioManager::stopEvent(const std::string& strEventName, bool bImmediate)
	{
	}

	void AudioManager::getEventParameter(const std::string& strEventName, const std::string& strEventParameter, float* value)
	{
	}

	void AudioManager::setEventParameter(const std::string& strEventName, const std::string& strParameterName, float* value)
	{
	}

	void AudioManager::setEvent3DAttributes(const std::string& strEventName, const glm::mat4& transform, const glm::vec3& velocity)
	{
	}

	void AudioManager::togglePauseAllChannels()
	{
	}

	void AudioManager::setChannel3dAttributes(int32_t nChannelId, const glm::vec3& position, const glm::vec3& velocity)
	{
	}

	bool AudioManager::isPlaying(int32_t nChannelID) const
	{
		return false;
	}

	bool AudioManager::isEventPlaying(const std::string& strEventName) const
	{
		return false;
	}

}