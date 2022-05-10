#pragma once
#include "entt/entt.hpp"
#include "engine.h"

using namespace Engine;

class DestroyAutoComponent : public Engine::NativeScript {
public:
	DestroyAutoComponent(entt::entity& entity, float timeToDestroy, int index) : NativeScript(entity), m_time(timeToDestroy), m_index(index)
	{

	};


	virtual void OnUpdate(float time)
	{
		clock += time;
		if (clock >= m_time)
		{
			auto world = Application::getInstance().GetWorld();
			auto& registry = Application::getInstance().m_registry;
			auto& entities = Application::getInstance().m_entities;

			entt::entity entity = entities[m_index];
			if (registry.valid(entity))
			{
				auto& rb = registry.get<RigidBodyComponent>(entity);
				world->destroyRigidBody(rb.m_body);
				registry.destroy(entity);
			}

		}
	}

private:
	float clock = 0.0f;
	float m_time = 0.0f;
	int m_index = 0.0f;
};
