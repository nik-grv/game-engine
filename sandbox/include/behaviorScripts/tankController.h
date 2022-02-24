#pragma once

#include "engine.h"

using namespace Engine;

class TankController : public Engine::NativeScript {
public:
	TankController(entt::entity& entity, float movementSpeed, bool active) :
		NativeScript(entity),
		m_movementSpeed(movementSpeed),
		m_active(active)
	{};

	virtual void OnCreate() override {

	};

	virtual void OnUpdate(float time)
	{
		entt::registry& registry = Application::getInstance().m_registry;
		auto& rb = registry.get<RigidBodyComponent>(m_entity);

		if (InputPoller::isKeyPressed(NG_KEY_W))
		{
			rb.m_body->applyForceToCenterOfMass(rp3d::Vector3(0.f, 0.f, 1.f) * m_movementSpeed);
		}

		if (InputPoller::isKeyPressed(NG_KEY_S))
		{
			rb.m_body->applyForceToCenterOfMass(rp3d::Vector3(0.f, 0.f, -1.f) * m_movementSpeed);
		}

		if (InputPoller::isKeyPressed(NG_KEY_A))
		{
			rb.m_body->applyTorque(rp3d::Vector3(0.f, -1.f, 0.f));
		}

		if (InputPoller::isKeyPressed(NG_KEY_D))
		{
			rb.m_body->applyTorque(rp3d::Vector3(0.f, 1.f, 0.f));
		}
	}

protected:
	float m_movementSpeed;
	float m_timeElapsed;
	bool m_active = true;
};