/*\file containers.h*/

#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "UI/Widgets.h"

namespace Engine
{
	class ModalWindow; 
	//class HorizontalContainer;

	class Container : public UIComponent
	{
	public: 
		virtual bool isEmpty() const = 0;
	};

	class HorizontalContainer : public Container
	{
	public:
		bool isEmpty() const override { return m_widgets.empty(); }
		template <typename G, typename ...Args>
		void addWidget(Args && ... args)
		{
			std::shared_ptr<Widget> widget;
			widget.reset(new G(std::forward<Args>(args) ...));
			widget->setparent(this);
			widget->m_colourScheme = m_colourScheme;
			m_widgets.push_back(widget);
			updateSize();
		}
		void updatePosition(glm::ivec2& relativePosition) override;
		void updateSize() override;
		void onRender() const override;

		std::vector<std::shared_ptr<Widget>>::iterator begin() { return m_widgets.begin(); }
		std::vector<std::shared_ptr<Widget>>::iterator end() { return m_widgets.end(); }
		std::vector<std::shared_ptr<Widget>>::const_iterator begin() const { return m_widgets.begin(); }
		std::vector<std::shared_ptr<Widget>>::const_iterator end() const { return m_widgets.end(); }
	private:
		std::vector<std::shared_ptr<Widget>> m_widgets;
	};


	class RootContainer : public Container
	{
	public:
		RootContainer() = default;
		RootContainer(ModalWindow* parent);

		virtual void onRender() const override;
		virtual bool isEmpty() const override { return m_containers.empty(); }
		virtual void addContainer(HorizontalContainer& container);
		void updatePosition(glm::ivec2& relativePosition) override;
		void updateSize() override;

		std::vector<HorizontalContainer>::iterator begin() { return m_containers.begin(); } //!< Begin iterator 
		std::vector<HorizontalContainer>::iterator end() { return m_containers.end(); } //!< End iterator 
		std::vector<HorizontalContainer>::const_iterator begin() const { return m_containers.begin(); }
		std::vector<HorizontalContainer>::const_iterator end() const { return m_containers.end(); }
	private:
		std::vector<HorizontalContainer> m_containers;
	};



}