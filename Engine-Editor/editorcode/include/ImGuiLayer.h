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
			void OnUpdate();
			void onEvemt(Event& event);
		private:
	};
}