#pragma once
#include <entt/entt.hpp>
#include "core/application.h"

namespace Engine
{

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel();

		void SetContext();

		void OnImGuiRender();
	private:
		entt::registry& m_registry = Application::getInstance().m_registry;
	};
}