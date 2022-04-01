/* \file audioManager.cpp*/
#include "engine_pch.h"

#include "systems/audioManager.h"
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
		for (auto& pair : m_events) errorCheck(pair.second->release());
		for (auto& pair : m_banks) errorCheck(pair.second->unload());
		errorCheck(m_studioSystem->unloadAll());
		errorCheck(m_studioSystem->flushCommands());
		errorCheck(m_studioSystem->release());

		for (auto& pair : m_sounds) errorCheck(pair.second->release());
		errorCheck(m_lowLevelSystem->close());
		errorCheck(m_lowLevelSystem->release());
	}

	void AudioManager::update()
	{
		std::vector<std::map<int32_t, FMOD::Channel*>::iterator> l_stoppedChannels;
		for (auto it = m_channels.begin(); it != m_channels.end(); ++it)
		{
			bool isPlaying = false;
			it->second->isPlaying(&isPlaying);
			if (!isPlaying)
			{
				l_stoppedChannels.push_back(it);
			}
		}
		for (auto& it : l_stoppedChannels)
		{
			m_channels.erase(it);
		}
	}

	void AudioManager::loadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags)
	{
		auto it = m_banks.find(strBankName);
		if (it != m_banks.end())
			return;
		FMOD::Studio::Bank* bank;
		errorCheck(m_studioSystem->loadBankFile(strBankName.c_str(), flags, &bank));
		if (bank) 
		{
			m_banks[strBankName] = bank;
		}
	}

	void AudioManager::loadEvent(const std::string& strEventName)
	{
		auto it = m_events.find(strEventName);
		if (it != m_events.end())
			return;
		FMOD::Studio::EventDescription* eventDescription = NULL;
		errorCheck(m_studioSystem->getEvent(strEventName.c_str(), &eventDescription));
		if (eventDescription)
		{
			FMOD::Studio::EventInstance* eventInstance = NULL;
			errorCheck(eventDescription->createInstance(&eventInstance));
			if (eventInstance)
			{
				m_events[strEventName] = eventInstance;
			}
		}
	}

	void AudioManager::loadSound(const std::string& strSoundName, bool b3d, bool bLooping, bool bStream, float minDist, float maxDist, RollOff rollOff)
	{
		auto it = m_sounds.find(strSoundName);
		if (it != m_sounds.end())
		{
			return;
		}
		FMOD_MODE eMode = FMOD_DEFAULT;
		eMode |= b3d ? FMOD_3D : FMOD_2D;
		eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;
		switch (rollOff)
		{
		case RollOff::Linear:
			eMode |= FMOD_3D_LINEARROLLOFF;
			break;
		case RollOff::LinearSquared:
			eMode |= FMOD_3D_LINEARSQUAREROLLOFF;
			break;
		case RollOff::InverseTapered:
			eMode |= FMOD_3D_INVERSETAPEREDROLLOFF;
			break;
		}
		FMOD::Sound* sound = nullptr;
		errorCheck(m_lowLevelSystem->createSound(strSoundName.c_str(), eMode, 0, &sound));

		if (b3d)
		{
			sound->set3DMinMaxDistance(minDist, maxDist);
		}
		if (sound != nullptr)
		{
			m_sounds[strSoundName] = sound;
		}
	}

	void AudioManager::unLoadSound(const std::string& strSoundName)
	{
		auto it = m_sounds.find(strSoundName);
		if (it != m_sounds.end())
			return;
		errorCheck(it->second->release());
		m_sounds.erase(it);
	}

	void AudioManager::set3dListenerAndOrientation(const glm::mat4& transform, const glm::vec3& velocity)
	{
		FMOD_VECTOR lastPos, lastVel, lastForward, lastUp;

		glm::vec3 up = { transform[1][0], transform[1][1], transform[1][2] };
		glm::vec3 forward = { transform[2][0], transform[2][1], transform[2][2] };
		glm::vec3 position = { transform[3][0], transform[3][1], transform[3][2] };

		auto listenerPos = GLMVecToFmod(position);
		auto listenerForward = GLMVecToFmod(forward);
		auto listenerUp = GLMVecToFmod(up);
		auto listenerVelocity = GLMVecToFmod(velocity);

		FMOD_3D_ATTRIBUTES f;
		f.position = listenerPos;
		f.forward = listenerForward;
		f.up = listenerUp;
		f.velocity = listenerVelocity;

		errorCheck(m_lowLevelSystem->set3DListenerAttributes(0, &listenerPos, &listenerVelocity, &listenerForward, &listenerUp));
		errorCheck(m_studioSystem->setListenerAttributes(0, &f));
	}

	void AudioManager::addGeometry(const std::string& label, const AudioGeometryDefinition& def)
	{
		FMOD::Geometry* geometry;

		int32_t numPolys = def.indices->size() / 3;

		errorCheck(m_lowLevelSystem->createGeometry(numPolys, def.vertices->size(), &geometry));

		m_geometry[label] = geometry;
		FMOD_VECTOR triangle[3];
		FMOD_VECTOR vert = { 0,0,0 };
		int32_t polygonIndex;

		for (int32_t i = 0, j = 0; i < def.indices->size(); i++)
		{
			vert = GLMVecToFmod(def.vertices->at(def.indices->at(i)));
			triangle[j] = vert;
			j++;

			if (j == 3)
			{
				geometry->addPolygon(def.directOcculsion, def.reverbOcculsion, true, 3, triangle, &polygonIndex);
				j = 0;
			}
		}
		geometry->setScale(&GLMVecToFmod(def.scale));
		geometry->setPosition(&GLMVecToFmod(def.position));
		geometry->setRotation(&GLMVecToFmod(def.forward), &GLMVecToFmod(def.up));
	}

	void AudioManager::moveGeometry(const std::string& label, const glm::vec3& position, const glm::vec3& forward, const glm::vec3& up, const glm::vec3& scale)
	{
		m_geometry[label]->setScale(&GLMVecToFmod(scale));
		m_geometry[label]->setPosition(&GLMVecToFmod(position));
		m_geometry[label]->setRotation(&GLMVecToFmod(forward), &GLMVecToFmod(up));
	}

	int32_t AudioManager::playSound(const std::string& strSoundName, const glm::vec3& vPos)
	{
		int32_t channelID = m_nextChannelId++;
		auto it = m_sounds.find(strSoundName);
		if (it == m_sounds.end())
		{
			loadSound(strSoundName);
			it = m_sounds.find(strSoundName);
			if (it == m_sounds.end());
			{
				return channelID;
			}
		}
		FMOD::Channel* channel = nullptr;
		errorCheck(m_lowLevelSystem->playSound(it->second, 0, true, &channel));
		if (channel)
		{
			FMOD_MODE currMode;
			it->second->getMode(&currMode);
			if (currMode & FMOD_3D)
			{
				FMOD_VECTOR FVposition = GLMVecToFmod(vPos);
				FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
				errorCheck(channel->set3DAttributes(&FVposition, &vel));
			}
			errorCheck(channel->setPaused(false));
			m_channels[channelID] = channel;
		}
		return channelID;
	}

	void AudioManager::playEvent(const std::string& strEventName)
	{
		auto it = m_events.find(strEventName);
		if (it == m_events.end())
		{
			loadEvent(strEventName);
			it = m_events.find(strEventName);
			if (it == m_events.end())
				return;
		}
		it->second->start();
	}

	void AudioManager::toggleChannelPause(int32_t nChannelId)
	{
		auto it = m_channels.find(nChannelId);
		if (it == m_channels.end())
			return;
		bool paused;
		errorCheck(it->second->getPaused(&paused));
		paused = !paused;
		errorCheck(it->second->setPaused(paused));
	}

	void AudioManager::stopEvent(const std::string& strEventName, bool bImmediate)
	{
		auto it = m_events.find(strEventName);
		if (it == m_events.end())
			return;
		FMOD_STUDIO_STOP_MODE eMode;
		eMode = bImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
		errorCheck(it->second->stop(eMode));
	}

	void AudioManager::getEventParameter(const std::string& strEventName, const std::string& strEventParameter, float* value)
	{
		auto it = m_events.find(strEventName);
		if (it == m_events.end())
			return;
		errorCheck(it->second->getParameterByName(strEventParameter.c_str(), value));
	}

	void AudioManager::setEventParameter(const std::string& strEventName, const std::string& strParameterName, float value)
	{
		auto it = m_events.find(strEventName);
		if (it == m_events.end())
			return;
		errorCheck(it->second->setParameterByName(strParameterName.c_str(), value));
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