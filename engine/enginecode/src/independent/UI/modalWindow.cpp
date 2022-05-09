/*\file modalWindow.cpp*/

#include "engine_pch.h"
#include "UI/modalWindow.h"
#include "Ui/containers.h"

namespace Engine
{
	int32_t ModalWindow::padding = 6;

	ModalWindow::ModalWindow()
	{
		glm::ivec2 winSize = glm::ivec2 (Application::getInstance().getAppWindow()->getWidth(), Application::getInstance().getAppWindow()->getHeight());
		m_position = (winSize / 2);
		m_size = (glm::ivec2(0.f));
		m_windowColourScheme = (UIColourScheme());
		m_colourScheme = &m_windowColourScheme;
		m_container = RootContainer(this);
		m_container.updatePosition(m_position + glm::ivec2(ModalWindow::padding, ModalWindow::padding));
	}

	void ModalWindow::onRender() const
	{
		if (m_active)
		{
			glm::vec2 winSize = glm::ivec2(Application::getInstance().getAppWindow()->getWidth(), Application::getInstance().getAppWindow()->getHeight());
			glm::vec4 bgCover = m_windowColourScheme.background;
			bgCover.a = 0.5f;
			Renderer2D::submit(Quad::createCentreHalfExtens({ 0.f,0.f }, winSize), bgCover);

			Renderer2D::submit(Quad::createCentreHalfExtens(m_position, m_size), m_windowColourScheme.background);

			m_container.onRender();
		}
	}
	
	void ModalWindow::addContainer(HorizontalContainer& container)
	{
		m_container.addContainer(container);
	}

	void ModalWindow::updateSize()
	{
		glm::ivec2 childSize = m_container.getSize();
		m_size.x = std::max(childSize.x + (padding * 2), m_size.x);
		m_size.y = std::max(childSize.y + (padding * 2), m_size.y);

		glm::ivec2 winSize = glm::ivec2(Application::getInstance().getAppWindow()->getWidth(), Application::getInstance().getAppWindow()->getHeight());

		m_position = (winSize - m_size) / 2;
		m_container.updatePosition(m_position + glm::ivec2(ModalWindow::padding, ModalWindow::padding));
	}

	void ModalWindow::onMouseBtnPressed(glm::ivec2& mousePosition, int32_t button)
	{
		for (auto& hzContainer : m_container)
		{
			for (auto& widget : hzContainer)
			{
				widget->onMouseBtnPressed(mousePosition, button);
			}
		}
	}

	void ModalWindow::onMouseBtnReleased(glm::ivec2& mousePosition, int32_t button)
	{
		for (auto& hzContainer : m_container)
		{
			for (auto& widget : hzContainer)
			{
				widget->onMouseBtnReleased(mousePosition, button);
			}
		}
	}

	void ModalWindow::onMouseMoved(glm::ivec2& mousePosition)
	{
		for (auto& hzContainer : m_container)
		{
			for (auto& widget : hzContainer)
			{
				widget->onMouseMoved(mousePosition);
			}
		}
	}
}