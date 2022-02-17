#pragma once

#include <functional>
#include <entt/entt.hpp>
#include "events/events.h"

namespace Engine {
	class NativeScript {
	public:
		NativeScript() : m_entity(entt::null) {};
		NativeScript(entt::entity& entity) : m_entity(entity) {};
		virtual ~NativeScript() = default;
		virtual void OnCreate() {};
		virtual void OnDestroy() {};
		virtual void OnUpdate(float timestep) {};
		virtual void OnKeyPress(KeyPressedEvent& e) {};
	private:
		entt::entity m_entity;
	};

	class NativeScriptComponent {
	public:
		NativeScriptComponent() = default;
		~NativeScriptComponent() = default;
		void OnUpdate(float timestep) { m_instance->OnUpdate(timestep); };
		void OnKeyPress(KeyPressedEvent& e) { m_instance->OnKeyPress(e); };

		template <typename G, typename ...Args> void create(Args && ... args) {
			G* ptr = new G(std::forward<Args>(args) ...);
			m_instance.reset(static_cast<NativeScript*>(ptr));
			m_instance->OnCreate();
		};
	protected:
		std::shared_ptr<NativeScript> m_instance;
	};

	namespace ScriptSystem {
		void UpdateScripts(float timestep);
		void OnKeyPressed(KeyPressedEvent& e);
	}
}