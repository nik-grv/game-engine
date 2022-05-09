#pragma once

#include "core/application.h"

namespace Engine
{
	class PlayableSoundComponent
	{
	public:
		PlayableSoundComponent(const char* soundFilePath, bool looping = true);
		void play(glm::vec3& location);
		void pause();

		const char* soundName;
		int32_t channel;
		bool isPlaying;
		bool setToLoop;
	};

	class ParametricSoundComponent
	{
	public:
		ParametricSoundComponent(const char* BankFilePath, const char* BankStringsFilePath, const char* eventName);
		std::string eventName;
	};

	class CurrentListenerComponent
	{
	public:
		CurrentListenerComponent() {}
	};

	class AudioOcculsionComponent
	{
	public:
		//AudioOcculsionComponent(const char* Pname, const AudioGeometryDefinition& geometryDef);
		//onst char* name;
	};

	namespace AudioSystem {
		void updatePositions();
	}
}