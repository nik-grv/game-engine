#pragma once

#include "rendering/RendererShared.h"
#include "systems/randomiser.h"

namespace Engine {

	enum class RandomTypes { None, Normal, Uniform };

	struct ParticleHostProperties {
		glm::vec3 linearVelocity = { 0.f, 0.f, 0.f };
		glm::vec3 linearAccelaration = { 0.f, 0.f, 0.f };
		glm::vec3 linearDrag = { 0.f, 0.f, 0.f };

		float angularVelocity = 0.f;
		float angularAcceleration = 0.f;
		float angularDrag = 0.f;

		float lifetime = 60.f;
		float lifetimeRemaining = 60.f;

		glm::vec4 startColor = { 0.f, 1.f, 1.f, 1.f };
		glm::vec4 endColor = { 0.f, 0.f, 0.f, 0.f };
		glm::vec2 startSize = { 1.f, 1.f };
		glm::vec2 endSize = { 1.f, 1.f };

		glm::vec3 posRandomisation = { 0.f, 0.f, 0.f };
		glm::vec3 velocityRandomisation = { 0.f, 0.f, 0.f };
		float scaleRandomisation = 0.f;

		glm::vec3 colorRandomisation = { 0.f, 0.f, 0.f };

		RandomTypes posRandomType = RandomTypes::None;
		RandomTypes velRandomType = RandomTypes::None;
		RandomTypes scaleRandomType = RandomTypes::None;
		RandomTypes colorRandomType = RandomTypes::None;

	};

	struct ParticleDeviceProperties {
		glm::vec3 linearPosition = { 0.f, 0.f, 0.f };
		float angularPosition = 0.f;
		glm::vec4 currentColor = glm::vec4(255);
		glm::vec2 currentSize = glm::vec2(1.f, 1.f);
		glm::vec2 current_UVStart = glm::vec2(0.f);
		glm::vec2 current_UVEnd = glm::vec2(1.f);
	};

	enum class BlendModes {None, Mix, Additive};

	class Particle {
	public:
		Particle(ParticleHostProperties& Hprops, ParticleDeviceProperties& Dprops, BlendModes p_blendMode = BlendModes::None);
		void OnUpdate(float timestep);
		ParticleHostProperties hostProps;
		ParticleDeviceProperties deviceProps;

		bool DestroyMe() { return hostProps.lifetimeRemaining <= 0.f; }
		BlendModes blendMode;
	};

	class ParticleVertex {
	public:
		ParticleVertex() = default;
		ParticleVertex(const glm::vec2& pos, const glm::vec3& pCentre, const glm::vec2& UVs, uint32_t textUnit, const glm::vec4& pTint) :
			position(pos), centre(pCentre), uvCoords(UVs), textureUnit(textUnit), tint(RendererShared::pack(pTint)) {}
		glm::vec2 position;
		glm::vec3 centre;
		glm::vec2 uvCoords;
		uint32_t textureUnit = 31;
		uint32_t tint;
		static VertexBufferLayout layout;
	};

	class ParticleSystem {
	public:
		static void init(uint32_t particleCapacity);
		static void AddParticle(Particle& p);
		static void OnUpdate(float timestep, glm::vec3& camera);
		static void OnRender(const SceneWideUniforms& swu);
		static bool AddTexture(const char* filepath, uint32_t& index);
		static bool GetUVs(uint32_t index, glm::vec2& uv_start, glm::vec2& uv_end);
	private:
		static void AddParticleRenderData(Particle& p);

		struct InternalData {
			std::shared_ptr<ShaderRend> shader;
			std::shared_ptr<VertexArray> VAO;
			std::shared_ptr<UniformBuffer> UBO;

			std::array<glm::vec4, 4> quad;
			uint32_t particleBatchSize;
			uint32_t particleCapacity;
			uint32_t nonBlendParticleDrawCount;
			uint32_t mixedBlendParticleDrawCount;
			uint32_t additiveBlendParticleDrawCount;

			std::vector<ParticleVertex> nonBlendVertecies;
			std::vector<ParticleVertex> mixedBlendVertecies;
			std::vector<ParticleVertex> additiveBlendVertecies;

			std::vector<Particle> nonBlendParticles;
			std::vector<Particle> mixedBlendParticles;
			std::vector<Particle> additiveBlendParticles;

			std::shared_ptr<TextureAtlas> atlas;
			std::vector<SubTexture> particleTexture;
			uint32_t textureUnit = 31;
		};

		static std::shared_ptr<InternalData> s_data;
	};

}