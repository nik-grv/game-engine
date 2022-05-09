/* \flie audioManager.h */
#pragma once

#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "glm/glm.hpp"
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>

#include "systems/system.h"

namespace Engine
{

	struct AudioGeometryDefinition {
		std::vector<glm::vec3>* vertices;
		std::vector<uint32_t>* indices;
		glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 forward = glm::vec3(0.f, 0.f, -1.f);
		glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
		glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f);
		float directOcculsion = 1.0f;
		float reverbOcculsion = 0.3f;
	};

	/**
		\class AudioManager
		holds and handles audio resources
		*/

	enum class RollOff { Inverse, InverseTapered, Linear, LinearSquared };
	class AudioManager : public System
	{
	private:
		FMOD::Studio::System* m_studioSystem;
		FMOD::System* m_lowLevelSystem;

		static const int32_t m_maxChannels = 32;
		int32_t m_nextChannelId = 0;

		std::map<std::string, FMOD::Sound*> m_sounds;
		std::map<int32_t, FMOD::Channel*> m_channels;
		std::map<std::string, FMOD::Geometry*> m_geometry;
		std::map<std::string, FMOD::Studio::EventInstance*> m_events;
		std::map<std::string, FMOD::Studio::Bank*> m_banks;
		int32_t errorCheck(FMOD_RESULT result) const;
		FMOD_VECTOR GLMVecToFmod(const glm::vec3& vec);

	public:
		void start(SystemSignal init = SystemSignal::None, ...); //!< start the system
		void stop(SystemSignal close = SystemSignal::None, ...); //!< stop the system
		void update();
		void loadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
		void loadEvent(const std::string& strEventName);
		void loadSound(const std::string &strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false, float minDist = 0.25f, float maxDist = 10000.f, RollOff rollOff = RollOff::Inverse);
		void unLoadSound(const std::string &strSoundName);
		void set3dListenerAndOrientation(const glm::mat4& transform, const glm::vec3& velocity);
		void addGeometry(const std::string& label, const AudioGeometryDefinition& def);
		void moveGeometry(const std::string& label, const glm::vec3& position, const glm::vec3& forward, const glm::vec3& up, const glm::vec3& scale);
		int32_t playSound(const std::string &strSoundName, const glm::vec3& vPos = glm::vec3{ 0, 0, 0 });
		void playEvent(const std::string& strEventName);
		void toggleChannelPause(int32_t nChannelId);
		void stopEvent(const std::string& strEventName, bool bImmediate = false);
		void getEventParameter(const std::string& strEventName, const std::string& strEventParameter, float* value);
		void setEventParameter(const std::string& strEventName, const std::string& strParameterName, float value);
		void setEvent3DAttributes(const std::string& strEventName, const glm::mat4& transform, const glm::vec3& velocity);
		void togglePauseAllChannels();
		void setChannel3dAttributes(int32_t nChannelId, const glm::vec3& position, const glm::vec3& velocity);
		bool isPlaying(int32_t nChannelID) const;
		bool isEventPlaying(const std::string& strEventName) const;
	};


}