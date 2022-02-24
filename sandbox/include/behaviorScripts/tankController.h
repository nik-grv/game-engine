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
		float desRotVel = 0.f;
		rp3d::Vector3 desSpeed = rp3d::Vector3(0.f, 0.f, 0.f);

		entt::registry& registry = Application::getInstance().m_registry;
		auto& rb = registry.get<RigidBodyComponent>(m_entity);
		auto& tc = registry.get<TransformComponent>(m_entity);
		auto& t = tc.GetTransform();
		rp3d::Vector3 forward(t[2][0], t[2][1], t[2][2]);

		if (InputPoller::isKeyPressed(NG_KEY_UP))
		{
			desSpeed = forward * m_movementSpeed;
		}

		if (InputPoller::isKeyPressed(NG_KEY_DOWN))
		{
			desSpeed = forward * -m_movementSpeed;
		}

		if (InputPoller::isKeyPressed(NG_KEY_LEFT))
		{
			desRotVel = -3.f;
		}

		if (InputPoller::isKeyPressed(NG_KEY_RIGHT))
		{
			desRotVel = 3.f;
		}

		float mass = rb.m_body->getMass();

		float deltaRotV = desRotVel - rb.m_body->getAngularVelocity().y;
		float rotForceY = mass * (deltaRotV / time);

		rp3d::Vector3 deltaV = desSpeed - rb.m_body->getLinearVelocity();
		rp3d::Vector3 moveForce =  (deltaV / time) * mass;

		rb.m_body->applyTorque(rp3d::Vector3(0.f, rotForceY, 0.f));
		rb.m_body->applyForceToCenterOfMass(moveForce);
	}

protected:
	float m_movementSpeed;
	float m_timeElapsed;
	bool m_active = true;
};