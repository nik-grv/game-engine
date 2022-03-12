#pragma once

#include "rendering/RendererShared.h"
//#include "systems/randomiser.h"

namespace Engine {
	struct ParticleHostProperties {
		glm::vec3 linearVelocity = { 0.f, 0.f, 0.f };
		glm::vec3 linearAccelaration = { 0.f, 0.f, 0.f };
		glm::vec3 linearDrag = { 0.f, 0.f, 0.f };

		float angularVelocity = 0.f;
		float angularAcceleration = 0.f;
		float angularDrag = 0.f;

		float lifetime = 1.f;
		float lifetimeRemaining = 1.f;

		glm::vec4 startColor = { 0.f, 1.f, 1.f, 1.f };
		glm::vec4 endColor = { 0.f, 0.f, 0.f, 0.f };
		glm::vec2 startSize = { 1.f, 1.f };
		glm::vec2 endSize = { 1.f, 1.f };

		glm::vec3 posRandomisation = { 0.f, 0.f, 0.f };
		glm::vec3 velocityRandomisation = { 0.f, 0.f, 0.f };
		float scaleRandomisation = 0.f;

		glm::vec3 colorRandomisation = { 0.f, 0.f, 0.f };

		//RandomTypes posRandomType = RandomTypes::None;
		//RandomTypes velRandomType = RandomTypes::None;
		//RandomTypes scaleRandomType = RandomTypes::None;
		//RandomTypes colorRandomType = RandomTypes::None;

	};

	struct ParticleDeviceProperties {
		glm::vec3 linearPosition = { 0.f, 0.f, 0.f };
		float angularPosition = 0.f;
		glm::vec4 currentColor;
		glm::vec2 currentSize;
		glm::vec2 current_UVStart;
		glm::vec2 current_UVEnd;
	};

	enum class BlendModes {None, Mix, Additive};

	class Particle {
		Particle::Particle(ParticleHostProperties& Hprops, ParticleDeviceProperties& Dprops, BlendModes p_blendMode = BlendModes::None);
		void OnUpdate(float timestep);
		ParticleHostProperties hostProps;
		ParticleDeviceProperties deviceProps;

		bool DestroyMe() { return hostProps.lifetimeRemaining <= 0.f; }
		BlendModes blendMode;
	};

	class ParticleVertex {
		ParticleVertex() = default;
		ParticleVertex(const glm::vec2& pos, const glm::vec3& pCentre, const glm::vec2& UVs, uint32_t textUnit, const glm::vec4& pTint) :
			position(pos), centre(pCentre), uvCoords(UVs), textureUnit(textUnit), tint(RendererShared::pack(pTint)) {}
		glm::vec2 position;
		glm::vec3 centre;
		glm::vec2 uvCoords;
		uint32_t textureUnit;
		uint32_t tint;
		static VertexBufferLayout layout;
	};


}