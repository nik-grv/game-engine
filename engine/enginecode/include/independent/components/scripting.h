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
		virtual void OnKeyPressed(KeyPressedEvent& e) {};
		virtual void OnMouseBtnPressed(MouseButtonPressedEvent& e) {};
		virtual void OnMouseMoved(MouseMovedEvent& e) {};
	protected:
		entt::entity m_entity;
	};

	class NativeScriptComponent {
	public:
		NativeScriptComponent() = default;
		~NativeScriptComponent() = default;
		void OnUpdate(float timestep) { m_instance->OnUpdate(timestep); };
		void OnKeyPressed(KeyPressedEvent& e) { m_instance->OnKeyPressed(e); };
		void OnMouseBtnPressed(MouseButtonPressedEvent& e) { m_instance->OnMouseBtnPressed(e); };
		void OnMouseMoved(MouseMovedEvent& e) { m_instance->OnMouseMoved(e); };

		template <typename G, typename ...Args> void create(Args && ... args) {
			G* ptr = new G(std::forward<Args>(args) ...);
			m_instance.reset(static_cast<NativeScript*>(ptr));
			m_instance->OnCreate();
		};
	protected:
		std::shared_ptr<NativeScript> m_instance = nullptr;
	};

	namespace ScriptSystem {
		void UpdateScripts(float timestep);
		void OnKeyPressed(KeyPressedEvent& e);
		void OnMouseBtnPressed(MouseButtonPressedEvent& e);
		void OnMouseMoved(MouseMovedEvent& e);

	}
}