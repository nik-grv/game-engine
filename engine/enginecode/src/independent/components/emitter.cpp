
#include "engine_pch.h"
#include <entt/entt.hpp>
#include "core/application.h"
#include "components/transform.h"
#include "components/emitter.h"
#include "components/rigidBody.h"
#include "systems/physics.h"


namespace Engine {
	namespace EmitterSystem {
		void Update(float timestep)
		{
			entt::registry& reg = Application::getInstance().m_registry;

			auto view = reg.view<EmitterComponent>();

			for (auto entity : view) {

				auto& [emitter, t] = reg.get<EmitterComponent, TransformComponent>(entity);
				auto& transform = t.GetTransform();

				if (!emitter.trackingVelocity) {
					glm::mat4 rotation = transform;
					rotation[3] = { 0.f, 0.f, 0.f, 1.f };

					glm::mat4 translation = glm::mat4(1.0f);
					translation[3] = transform[3];
					glm::vec4 posDelta(emitter.positionOffset, 0.f);
					translation[3] += posDelta;

					glm::mat4 particleTransform = translation * rotation;

					emitter.deviceProps.linearPosition = particleTransform[3];
				}
				else {
					auto& rb = reg.get<RigidBodyComponent>(entity);
					auto rbVel = rb.m_body->getLinearVelocity();

					glm::vec3 vel(rbVel.x, rbVel.y, rbVel.z);
					float mag = glm::length(vel);

					if (mag > 0.f) {
						vel = -glm::normalize(vel);
						emitter.deviceProps.linearPosition = glm::vec3(transform[3]) + (vel * emitter.positionOffset.y);
						emitter.hostProps.linearVelocity = vel * glm::length(emitter.hostProps.linearVelocity);
					}

					else {
						emitter.elapsedTime = 0.f;
					}
				}

				emitter.elapsedTime += timestep;

				while (emitter.elapsedTime > emitter.timeBetweenEmissions) {
					Particle p(emitter.hostProps, emitter.deviceProps, emitter.blendModes);
					ParticleSystem::AddParticle(p);

					emitter.elapsedTime -= emitter.timeBetweenEmissions;
				}
			}
		}
	}
}