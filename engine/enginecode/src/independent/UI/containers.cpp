/*\file containers.cpp*/

#include "engine_pch.h"
#include "UI/modalWindow.h"

namespace Engine
{
	RootContainer::RootContainer(ModalWindow* parent)
	{
		m_parent = parent; 
		m_colourScheme = &parent->m_windowColourScheme;
		m_position = { 0,0 };
		m_size = { ModalWindow::padding * 2, ModalWindow::padding * 2 };

	}
	
	void RootContainer::onRender() const
	{
		for (auto& container : m_containers) container.onRender();
	}

	void RootContainer::addContainer(HorizontalContainer& container)
	{
		container.setparent(this);
		container.m_colourScheme = m_colourScheme;

		for (auto& widget : container) { widget->m_colourScheme = m_colourScheme; }
		
		m_containers.push_back(container);
		updateSize();
	}

	void RootContainer::updatePosition(glm::ivec2& relativePosition)
	{
		m_position = relativePosition;

		glm::ivec2 childRelativePosition = m_position + glm::ivec2(ModalWindow::padding, ModalWindow::padding);
		for (auto& container : m_containers)
		{
			container.updatePosition(childRelativePosition);
			childRelativePosition.y += container.getSize().y;
		}
	}
	void RootContainer::updateSize()
	{
		glm::ivec2 size(0);
		size += 2 * glm::ivec2(ModalWindow::padding);
		for (auto& container : m_containers)
		{
			size.x = std::max(size.x, ModalWindow::padding * 2 + container.getSize().x);
			size.y += container.getSize().y;
		}
		m_size = size;

		m_parent->updateSize();

	}

	void HorizontalContainer::updatePosition(glm::ivec2& relativePosition)
	{
		m_position = relativePosition; 
		glm::ivec2 childRelativePosition = m_position + glm::ivec2(ModalWindow::padding, ModalWindow::padding);
		for (auto& widget : m_widgets)
		{
			widget->updatePosition(childRelativePosition);
			childRelativePosition.x += widget->getSize().x;
		}
	}

	void HorizontalContainer::updateSize()
	{
		glm::ivec2 size(0);
		size += 2 * glm::ivec2(ModalWindow::padding, ModalWindow::padding);
		for (auto& widget : m_widgets)
		{
			size.x += widget->getSize().x;
			size.y = std::max(size.y, ModalWindow::padding * 2 + widget->getSize().y);
		}
		m_size = size;
		
		if (m_parent) m_parent->updateSize();
	}

	void HorizontalContainer::onRender() const
	{
		for (auto widget : m_widgets) widget->onRender();
	}

}