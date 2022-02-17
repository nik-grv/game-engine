#pragma once

#include "engine.h"

namespace Engine {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(const char* name);
		~ImGuiLayer();

		void OnAttach();
		void OnDettach();
		void OnUpdate(float timestep)override;

		void onKeyPressed(KeyPressedEvent& e) override;
		void onKeyReleased(KeyReleasedEvent& e) override;
		//void onKeyTypedEvent(KeyTypedEvent& e) override; (IMPLEMENT FUCNTION LATER)

		void onMouseMoved(MouseMovedEvent& e) override;
		void onMouseBtnPressed(MouseButtonPressedEvent& e) override;
		void onMouseBtnReleased(MouseButtonReleasedEvent& e) override;
		void onMouseScrolled(MouseScrollEvent& e) override;
		Application& app = Application::getInstance();
	private:

	private:
		float m_Time = 0.0f;
	};
}