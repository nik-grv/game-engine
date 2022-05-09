#pragma once

#include "engine.h"
#include <assimpLoader.h>

using namespace Engine;

class EnemyTank : public Engine::NativeScript {

public:
	EnemyTank(entt::entity& entity, float movementSpeed, bool moveX, float offset) : NativeScript(entity),
		m_speed(movementSpeed),
		m_offset(offset),
		m_moveX(moveX)
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


		if (m_moveX)
		{
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
				rb.m_body->setLinearVelocity(desSpeed);
			}
			else
			{
				rb.m_body->setLinearVelocity(-desSpeed);
			}
		}
		else
		{
			rp3d::Vector3 desSpeed = rp3d::Vector3(0.f, 0.f, m_speed);

			if (tc.translation.z >= startPos.z + m_offset)
			{
				right = false;
				left = true;
			}
			else if (tc.translation.z <= startPos.z - m_offset)
			{
				right = true;
				left = false;
			}

			if (right)
			{
				rb.m_body->setLinearVelocity(desSpeed);
			}
			else
			{

				rb.m_body->setLinearVelocity(-desSpeed);
			}
		}
	}


private:
	float m_speed = 0.0f;
	bool m_active;
	bool left = false;
	bool right = true;
	float m_offset;
	bool m_moveX;
	glm::vec3 startPos;
};