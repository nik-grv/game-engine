/*\file modalWindow.h*/

#pragma once

#include "rendering/Renderer2D.h"
#include "core/application.h"
#include "UI/UIComponent.h"
#include "UI/containers.h"

namespace Engine
{
	class ModalWindow : public UIComponent
	{
	public:
		ModalWindow();

		UIColourScheme m_windowColourScheme;
		void show() { m_active = true; }
		void hide() { m_active = false; }
		void onRender() const override;
		void addContainer(HorizontalContainer& container);
		virtual void updateSize() override;
		virtual void updatePosition(glm::ivec2& relativePosition) override {};


		
		virtual void onMouseBtnPressed(glm::ivec2& mousePosition, int32_t button) override;
		virtual void onMouseBtnReleased(glm::ivec2& mousePosition, int32_t button) override;
		virtual void onMouseMoved(glm::ivec2& mousePosition) override;

		bool isShown() { return m_active; }
		static int32_t padding;
	private:
		bool m_active = false;
		RootContainer m_container;
	};
}