#include "engine_pch.h"
#include "components/scripting.h"
#include "core/application.h"
#include <entt/entt.hpp>

namespace Engine {

	void ScriptSystem::UpdateScripts(float time) {
		entt::registry& registry = Application::getInstance().m_registry;

		auto scriptView = registry.view<NativeScriptComponent>();

		for (auto entity : scriptView) {
			auto& nsc = registry.get<NativeScriptComponent>(entity);
			nsc.OnUpdate(time);
		}
	}

	void ScriptSystem::OnKeyPressed(KeyPressedEvent& e) {
		entt::registry& registry = Application::getInstance().m_registry;

		auto scriptView = registry.view<NativeScriptComponent>();

		for (auto entity : scriptView) {
			auto& nsc = registry.get<NativeScriptComponent>(entity);
			nsc.OnKeyPress(e);
		}
	}

}