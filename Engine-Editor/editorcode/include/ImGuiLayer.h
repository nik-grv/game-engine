#pragma once

#include "engine.h"

namespace Engine {

	class ImGuiLayer : public Layer
	{
		public:
			ImGuiLayer();
			~ImGuiLayer();

			void OnAttach();
			void OnDettach();
			void OnUpdate(float timestep)override;
			void onMouseMoved(MouseMovedEvent& e) override;
			void onKeyPressed(KeyPressedEvent& e) override;
		private:
			float m_Time = 0.0f;
	};
}