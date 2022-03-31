#pragma once
#include "entt/entt.hpp"
#include "application.h"


namespace Engine
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(float ts);
	private:
		entt::registry& m_Registry = Application::getInstance().m_registry;
	};
}