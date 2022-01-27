/** \file LayerStack.h*/
#pragma once

#include <vector>
#include <memory>
#include "core/Layer.h"
namespace Engine
{
	class LayerStack
	{
	public:
		LayerStack() { m_layers.reserve(10); }
		~LayerStack();
		void Update(float timeStep);
		void Render();

		void Push(Layer* layer);
		void Pop();

		void onKeyPressed(KeyPressedEvent& e); //!<run when keyboard button is pressed
		void onKeyReleased(KeyReleasedEvent& e); //!<run when keyboard button is released
		void onMouseMoved(MouseMovedEvent& e); //!<run when mouse is moved
		void onMouseBtnPressed(MouseButtonPressedEvent& e); //!<run when mouse button is pressed
		void onMouseBtnReleased(MouseButtonReleasedEvent& e); //!<run when mouse button is released
		void onMouseScrolled(MouseScrollEvent& e); //!<run when mouse button is released

		void SetActive(const char* layerName, bool state);
		void SetAllActive(bool state);

		std::vector<std::shared_ptr<Layer>>::iterator begin() { return m_layers.begin(); }
		std::vector<std::shared_ptr<Layer>>::iterator end() { return m_layers.end(); }
		std::vector<std::shared_ptr<Layer>>::reverse_iterator rBegin() { return m_layers.rbegin(); }
		std::vector<std::shared_ptr<Layer>>::reverse_iterator rEnd() { return m_layers.rend(); }

		std::vector<std::shared_ptr<Layer>>::const_iterator begin() const{ return m_layers.begin(); }
		std::vector<std::shared_ptr<Layer>>::const_iterator end() const { return m_layers.end(); }
		std::vector<std::shared_ptr<Layer>>::const_reverse_iterator rBegin() const { return m_layers.rbegin(); }
		std::vector<std::shared_ptr<Layer>>::const_reverse_iterator rEnd() const { return m_layers.rend(); }
	private:
		std::vector<std::shared_ptr<Layer>> m_layers;
	};

}