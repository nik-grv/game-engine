#pragma once

#include "engine.h"
#include <assimpLoader.h>

using namespace Engine;

class EnemyTank : public Engine::NativeScript {

public:
	EnemyTank(entt::entity& entity, float movementSpeed, bool active, float offset) : NativeScript(entity),
		m_speed(movementSpeed),
		m_active(active),
		m_offset(offset)
	{
		entt::registry& registry = Application::getInstance().m_registry;
		auto& tc = registry.get<TransformComponent>(m_entity);
		startPos = tc.translation;
	};

	virtual void OnUpdate(float time)
	{
		entt::registry& registry = Application::getInstance().m_registry;
		auto& rb = registry.get<RigidBodyComponent>(m_entity);
		auto& tc = registry.get<TransformComponent>(m_entity);
		auto& t = tc.GetTransform();
		rp3d::Vector3 desSpeed = rp3d::Vector3(m_speed, 0.f, 0.f);


		if (tc.translation.x >= startPos.x + m_offset)
		{
			right = false;
			left = true;
		}
		else if (tc.translation.x <= startPos.x - m_offset)
		{
			right = true;
			left = false;
		}

		if (right)
		{
			Log::error("{0} - moving righttttt", tc.translation.x);
			rb.m_body->setLinearVelocity(desSpeed);//->applyForceToCenterOfMass(moveForce);
		}
		else 
		{

			Log::error("{0} - moving lefttttttttttt", tc.translation.x);
			rb.m_body->setLinearVelocity(-desSpeed);//->applyForceToCenterOfMass(moveForce);
		}
	}
private:
	float m_speed = 0.0f;
	bool m_active;
	bool left = false;
	bool right = true;
	float m_offset;

	glm::vec3 startPos;
};