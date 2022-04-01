#include "engine_pch.h"
#include "systems/log.h"
#include <glad/glad.h>
#include "rendering/particleSystem.h"
#include "components/emitter.h"
#include "core/application.h"

#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <numeric>

namespace Engine {

	VertexBufferLayout ParticleVertex::layout = VertexBufferLayout({ ShaderDataType::Float2, ShaderDataType::Float3, ShaderDataType::Float2, ShaderDataType::FlatInt,  {ShaderDataType::Byte4} }); // 0 for instance

	std::shared_ptr<ParticleSystem::InternalData> ParticleSystem::s_data = nullptr;

	void ParticleSystem::init(uint32_t particleCapacity) {
		//auto& resources = Application::getInstance().getResourceManager(); // we dont have that yet

		s_data.reset(new InternalData);
		s_data->particleBatchSize = particleCapacity * 4;
		s_data->particleCapacity = particleCapacity;
		s_data->nonBlendParticleDrawCount = 0;
		s_data->mixedBlendParticleDrawCount = 0;
		s_data->additiveBlendParticleDrawCount = 0;

		//check if default texture is there if not, create it. Simon uses default texture of rendererCommon, we dont seem to have that.

		s_data->shader.reset(ShaderRend::create("./assets/shaders/quadBillboard.glsl")); // add shader later or use resource manager

		s_data->UBO.reset(UniformBuffer::create(UniformBufferLayout({
			{"u_view", ShaderDataType::Mat4},
			{"u_projection", ShaderDataType::Mat4}
			})));

	/*	for (int i = 0; i < s_data->textureUnit.size(); i++)
		{
			s_data->textureUnit[i] = i;
		}*/

		s_data->quad[0] = { -0.5f, -0.5f, 0.f, 1.f };
		s_data->quad[1] = { -0.5f, 0.5f, 0.f, 1.f };
		s_data->quad[2] = { 0.5f, 0.5f, 0.f, 1.f };
		s_data->quad[3] = { 0.5f, -0.5f, 0.f, 1.f };

		s_data->nonBlendVertecies.resize(s_data->particleBatchSize); //add mixed and additive blend later
		s_data->mixedBlendVertecies.resize(s_data->particleBatchSize); //add mixed and additive blend later
		s_data->additiveBlendVertecies.resize(s_data->particleBatchSize);
		//add mixed and additive blend later
		s_data->nonBlendParticles.reserve(s_data->particleCapacity); //add mixed and additive blend later
		s_data->mixedBlendParticles.reserve(s_data->particleCapacity); //add mixed and additive blend later
		s_data->additiveBlendParticles.reserve(s_data->particleCapacity); //add mixed and additive blend later


		std::vector<uint32_t> indices(s_data->particleBatchSize);
		std::iota(indices.begin(), indices.end(), 0);

		std::shared_ptr<VertexBuffer> VBO;
		std::shared_ptr<IndexBuffer> IBO;

		s_data->VAO.reset(VertexArray::create());
		VBO.reset(VertexBuffer::create(s_data->nonBlendVertecies.data(), sizeof(ParticleVertex) * s_data->nonBlendVertecies.size(), ParticleVertex::layout));
		IBO.reset(IndexBuffer::create(indices.data(), indices.size()));
		s_data->VAO->addVertexBuffer(VBO);
		s_data->VAO->setIndexBuffer(IBO);

		s_data->UBO->attachShaderBlock(s_data->shader, "b_camera");

		s_data->atlas.reset(new TextureAtlas({ 4096, 4096 }, 4, 100));
		s_data->particleTexture.reserve(100);
	}

	void ParticleSystem::AddParticle(Particle& p) {
		switch (p.blendMode) {
		case BlendModes::None:
			if (s_data->nonBlendParticleDrawCount < s_data->particleBatchSize) s_data->nonBlendParticles.push_back(p);
			break;
		case BlendModes::Mix:
			if (s_data->mixedBlendParticleDrawCount < s_data->particleBatchSize) s_data->mixedBlendParticles.push_back(p);
			break;
		case BlendModes::Additive:
			if (s_data->additiveBlendParticleDrawCount < s_data->particleBatchSize) s_data->additiveBlendParticles.push_back(p);
			break;
		}
	}

	void ParticleSystem::AddParticleRenderData(Particle& p) {

		auto packedTint = RendererShared::pack(p.deviceProps.currentColor);
		glm::mat4 model = glm::scale(glm::rotate(glm::mat4(1.f), p.deviceProps.angularPosition, { 0.f, 0.f, 1.f }), glm::vec3(p.deviceProps.currentSize, 1.f));

		std::vector<ParticleVertex>* pRenderVerts = nullptr;
		uint32_t startIndex = -1;

		switch (p.blendMode) {
		case BlendModes::None:
			pRenderVerts = &s_data->nonBlendVertecies;
			startIndex = s_data->nonBlendParticleDrawCount;
			s_data->nonBlendParticleDrawCount += 4;
			break;
		case BlendModes::Mix:
			pRenderVerts = &s_data->mixedBlendVertecies;
			startIndex = s_data->mixedBlendParticleDrawCount;
			s_data->mixedBlendParticleDrawCount += 4;
			break;
		case BlendModes::Additive:
			pRenderVerts = &s_data->additiveBlendVertecies;
			startIndex = s_data->additiveBlendParticleDrawCount;
			s_data->additiveBlendParticleDrawCount += 4;
			break;
		}
		Log::error("DRW COUNT - {0}", s_data->additiveBlendParticleDrawCount);

		for (int i = 0; i < 4; i++) {
			(*pRenderVerts)[i + startIndex].position = model * s_data->quad[i];
			(*pRenderVerts)[i + startIndex].centre = p.deviceProps.linearPosition;
			(*pRenderVerts)[i + startIndex].tint = packedTint;
			(*pRenderVerts)[i + startIndex].textureUnit = s_data->textureUnit;
		}

		(*pRenderVerts)[startIndex + 0].uvCoords = p.deviceProps.current_UVStart;
		(*pRenderVerts)[startIndex + 1].uvCoords = { p.deviceProps.current_UVStart.x, p.deviceProps.current_UVEnd.y };
		(*pRenderVerts)[startIndex + 2].uvCoords = p.deviceProps.current_UVEnd;
		(*pRenderVerts)[startIndex + 3].uvCoords = { p.deviceProps.current_UVEnd.x, p.deviceProps.current_UVStart.y };

	}

	void ParticleSystem::OnUpdate(float timestep, glm::vec3& camera) {

		EmitterSystem::Update(timestep); //Update particle emitter system / emits particles

		s_data->nonBlendParticleDrawCount = 0;
		for (int i = 0; i < s_data->nonBlendParticles.size(); i++) {
			s_data->nonBlendParticles[i].OnUpdate(timestep);

			if (s_data->nonBlendParticles[i].DestroyMe()) {
				if (i != s_data->nonBlendParticles.size() - 1) {
					s_data->nonBlendParticles[i] = s_data->nonBlendParticles[s_data->nonBlendParticles.size() - 1];
				}
				s_data->nonBlendParticles.pop_back();
				i--;
			}

			if (1 < i && i < s_data->particleCapacity) ParticleSystem::AddParticleRenderData(s_data->nonBlendParticles[i]);
			else if (i >= (int)s_data->particleCapacity) {
				Log::error("Non Blend Particles full");
			}
		}

		std::sort(s_data->mixedBlendParticles.begin(), s_data->mixedBlendParticles.begin() + s_data->mixedBlendParticleDrawCount,
			[camera](const Particle& p1, const Particle& p2) -> bool
			{
				return glm::length2(camera - p1.deviceProps.linearPosition) < glm::length2(camera - p2.deviceProps.linearPosition);
			});
		s_data->mixedBlendParticleDrawCount = 0;

		for (int i = 0; i < s_data->mixedBlendParticles.size(); i++) {
			s_data->mixedBlendParticles[i].OnUpdate(timestep);

			if (s_data->mixedBlendParticles[i].DestroyMe()) {
				if (i != s_data->mixedBlendParticles.size() - 1) {
					s_data->mixedBlendParticles[i] = s_data->mixedBlendParticles[s_data->mixedBlendParticles.size() - 1];
				}
				s_data->mixedBlendParticles.pop_back();
				i--;
			}
			if (1 < i && i < s_data->particleCapacity) ParticleSystem::AddParticleRenderData(s_data->mixedBlendParticles[i]);
			else if (i >= (int)s_data->particleCapacity) {
				Log::error("Mix Blend Particles full");
			}

		}

		std::sort(s_data->additiveBlendParticles.begin(), s_data->additiveBlendParticles.begin() + s_data->mixedBlendParticleDrawCount,
			[camera](const Particle& p1, const Particle& p2) -> bool
			{
				return glm::length2(camera - p1.deviceProps.linearPosition) < glm::length2(camera - p2.deviceProps.linearPosition);
			});
		s_data->additiveBlendParticleDrawCount = 0;

		for (int i = 0; i < s_data->additiveBlendParticles.size(); i++) {
			s_data->additiveBlendParticles[i].OnUpdate(timestep);

			if (s_data->additiveBlendParticles[i].DestroyMe()) {
				if (i != s_data->additiveBlendParticles.size() - 1) {
					s_data->additiveBlendParticles[i] = s_data->additiveBlendParticles[s_data->additiveBlendParticles.size() - 1];
				}
				s_data->additiveBlendParticles.pop_back();
				i--;
			}
			if (1 < i && i < s_data->particleCapacity) ParticleSystem::AddParticleRenderData(s_data->additiveBlendParticles[i]);
			else if (i >= (int)s_data->particleCapacity) {
				Log::error("Mix Blend Particles full");
			}

		}


	}

	void ParticleSystem::OnRender(const SceneWideUniforms& swu) {

		Log::error("DRW COUNT on rendr- {0}", s_data->additiveBlendParticleDrawCount);

		glUseProgram(s_data->shader->getRenderID());
		//s_data->shader->uploadIntArray("u_texData", &s_data->textureUnit,32); //sort this to be correct

		glBindBuffer(GL_UNIFORM_BUFFER, s_data->UBO->getRenderID());
		s_data->UBO->uploadDataToUB("u_projection", swu.at("u_projection").second);
		s_data->UBO->uploadDataToUB("u_view", swu.at("u_view").second);

	
		glActiveTexture(GL_TEXTURE0 + s_data->textureUnit);
		glBindTexture(GL_TEXTURE_2D, s_data->atlas->getBaseTexture()->getRenderID());

		s_data->VAO->getVertexBuffers().at(0)->edit(s_data->nonBlendVertecies.data(), sizeof(ParticleVertex) * s_data->nonBlendParticleDrawCount,0);
		glDrawElements(GL_QUADS, s_data->nonBlendParticleDrawCount, GL_UNSIGNED_INT, nullptr);
		s_data->nonBlendParticleDrawCount = 0;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		s_data->VAO->getVertexBuffers().at(0)->edit(s_data->mixedBlendVertecies.data(), sizeof(ParticleVertex) * s_data->mixedBlendParticleDrawCount,0);
		glDrawElements(GL_QUADS, s_data->mixedBlendParticleDrawCount, GL_UNSIGNED_INT, nullptr);
		s_data->mixedBlendParticleDrawCount= 0;

		glBlendFunc(GL_ONE, GL_ONE);
		s_data->VAO->getVertexBuffers().at(0)->edit(s_data->additiveBlendVertecies.data(), sizeof(ParticleVertex) * s_data->additiveBlendParticleDrawCount,0);
		glDrawElements(GL_QUADS, s_data->additiveBlendParticleDrawCount, GL_UNSIGNED_INT, nullptr);
		s_data->additiveBlendParticleDrawCount = 0;
		glDisable(GL_BLEND);
	}

	bool ParticleSystem::AddTexture(const char* filepath, uint32_t& index) {
		SubTexture t;
		if (s_data->atlas->add(filepath, t)) {
			s_data->particleTexture.push_back(t);
			index = s_data->particleTexture.size() - 1;
			return true;
		}
		index = 0xFFFFFFF;
		return false;
	}

	bool ParticleSystem::GetUVs(uint32_t index, glm::vec2& uv_start, glm::vec2& uv_end) {
		if (index < s_data->particleTexture.size()) {
			uv_start = s_data->particleTexture[index].getUVStart();
			uv_end = s_data->particleTexture[index].getUVEnd();
			return true;
		}
		return false;
	}

	Particle::Particle(ParticleHostProperties& Hprops, ParticleDeviceProperties& Dprops, BlendModes p_blendMode) :
		hostProps(Hprops), deviceProps(Dprops), blendMode(p_blendMode)
	{
		float r, g, b, a, scale;
		glm::vec4 delta4;
		glm::vec3 delta3;

		switch (hostProps.colorRandomType) {
		case RandomTypes::None:
			deviceProps.currentColor = hostProps.startColor;
			break;
		case RandomTypes::Normal:
			r = Randomiser::normalFloat(0, hostProps.colorRandomisation.x);
			g = Randomiser::normalFloat(0, hostProps.colorRandomisation.y);
			b = Randomiser::normalFloat(0, hostProps.colorRandomisation.z);
			delta4 = glm::vec4(r, g, b, 0.f);
			hostProps.startColor += delta4;
			hostProps.endColor -= delta4;
			deviceProps.currentColor = hostProps.startColor;
			break;
		case RandomTypes::Uniform:
			r = Randomiser::uniformFloatBetween(-hostProps.colorRandomisation.x, hostProps.colorRandomisation.x);
			g = Randomiser::uniformFloatBetween(-hostProps.colorRandomisation.y, hostProps.colorRandomisation.y);
			b = Randomiser::uniformFloatBetween(-hostProps.colorRandomisation.z, hostProps.colorRandomisation.z);
			delta4 = glm::vec4(r, g, b, 0.f);
			hostProps.startColor += delta4;
			hostProps.endColor -= delta4;
			deviceProps.currentColor = hostProps.startColor;
			break;
		}

		switch (hostProps.scaleRandomType) {
		case RandomTypes::None:
			deviceProps.currentSize = hostProps.startSize;
			break;
		case RandomTypes::Normal:
			scale = Randomiser::normalFloat(-hostProps.scaleRandomisation, hostProps.scaleRandomisation);
			hostProps.startSize -= scale;
			hostProps.endSize += scale;
			deviceProps.currentSize = hostProps.startSize;
			break;
		case RandomTypes::Uniform:
			scale = Randomiser::normalFloat(1.0, hostProps.scaleRandomisation);
			hostProps.startSize -= scale;
			hostProps.endSize += scale;
			deviceProps.currentSize = hostProps.startSize;
			break;
		}

		switch (hostProps.posRandomType) {
		case RandomTypes::None:
			break;
		case RandomTypes::Normal:
			delta3.x = Randomiser::normalFloat(deviceProps.linearPosition.x, hostProps.posRandomisation.x);
			delta3.y = Randomiser::normalFloat(deviceProps.linearPosition.y, hostProps.posRandomisation.y);
			delta3.z = Randomiser::normalFloat(deviceProps.linearPosition.z, hostProps.posRandomisation.z);
			deviceProps.linearPosition = delta3;
			break;
		case RandomTypes::Uniform:
			delta3.x = Randomiser::uniformFloatBetween(-hostProps.posRandomisation.x, hostProps.posRandomisation.x);
			delta3.y = Randomiser::uniformFloatBetween(-hostProps.posRandomisation.y, hostProps.posRandomisation.y);
			delta3.z = Randomiser::uniformFloatBetween(-hostProps.posRandomisation.z, hostProps.posRandomisation.z);
			deviceProps.linearPosition = delta3;
			break;
		}

		switch (hostProps.velRandomType) {
		case RandomTypes::None :
			break;
		case RandomTypes::Normal:
			delta3.x = Randomiser::normalFloat(hostProps.linearVelocity.x, hostProps.velocityRandomisation.x);
			delta3.y = Randomiser::normalFloat(hostProps.linearVelocity.y, hostProps.velocityRandomisation.y);
			delta3.z = Randomiser::normalFloat(hostProps.linearVelocity.z, hostProps.velocityRandomisation.z);
			hostProps.linearVelocity  = delta3;
			break;
		case RandomTypes::Uniform:
			delta3.x = Randomiser::uniformFloatBetween(-hostProps.velocityRandomisation.x, hostProps.velocityRandomisation.x);
			delta3.y = Randomiser::uniformFloatBetween(-hostProps.velocityRandomisation.y, hostProps.velocityRandomisation.y);
			delta3.z = Randomiser::uniformFloatBetween(-hostProps.velocityRandomisation.z, hostProps.velocityRandomisation.z);
			deviceProps.linearPosition = delta3;
			break;
		}
	}

	void Particle::OnUpdate(float timestep) {
		hostProps.linearVelocity += hostProps.linearAccelaration * timestep + hostProps.linearDrag * -hostProps.linearVelocity * timestep;
		deviceProps.linearPosition += hostProps.linearAccelaration * timestep;

		hostProps.angularAcceleration += hostProps.angularAcceleration * timestep + hostProps.angularDrag * -hostProps.angularVelocity * timestep;
		deviceProps.angularPosition += hostProps.angularVelocity;

		hostProps.lifetimeRemaining -= timestep;

		float lifetime_t = 1.0f - (hostProps.lifetimeRemaining / hostProps.lifetime);
	}

}