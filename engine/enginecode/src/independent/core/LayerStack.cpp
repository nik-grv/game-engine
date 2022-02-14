/** \file LayerStack.cpp*/

#include "engine_pch.h"
#include "core\LayerStack.h"

namespace Engine
{
	LayerStack::~LayerStack()
	{
		for (int i = m_layers.size(); i > 0 ; i--)
		{
			Pop();
		}
	}

	void LayerStack::Update(float timeStep)
	{
		for (auto& layer : m_layers)
		{
			if (layer->isActive())
				layer->OnUpdate(timeStep);
		}
	}

	void LayerStack::Render()
	{
		for (auto& layer : m_layers)
		{
			layer->OnRender();
		}
	}

	void LayerStack::Push(Layer* layer)
	{
		layer->OnAttach();
		std::shared_ptr<Layer> tmp(layer);
		m_layers.push_back(tmp);
	}

	void LayerStack::Pop()
	{
		m_layers.back()->OnDettach();
		m_layers.pop_back();
	}

	void LayerStack::onKeyPressed(KeyPressedEvent& e)
	{
		for (auto& layer : m_layers)
		{
			layer->onKeyPressed(e);
		}
	}

	void LayerStack::onKeyReleased(KeyReleasedEvent& e)
	{
		for (auto& layer : m_layers)
		{
			layer->onKeyReleased(e);
		}
	}

	void LayerStack::onMouseMoved(MouseMovedEvent& e)
	{
		for (auto& layer : m_layers)
		{
			layer->onMouseMoved(e);
		}
	}

	void LayerStack::onMouseBtnPressed(MouseButtonPressedEvent& e)
	{
		for (auto& layer : m_layers)
		{
			layer->onMouseBtnPressed(e);
		}
	}

	void LayerStack::onMouseBtnReleased(MouseButtonReleasedEvent& e)
	{
		for (auto& layer : m_layers)
		{
			layer->onMouseBtnReleased(e);
		}
	}

	void LayerStack::onMouseScrolled(MouseScrollEvent& e)
	{
		for (auto& layer : m_layers)
		{
			layer->onMouseScrolled(e);
		}
	}

	void LayerStack::SetActive(const char* layerName, bool state)
	{
		for (auto& layer : m_layers)
		{
			if(layer->getLayerName() == layerName)
			layer->setActive(state);
		}
	}

	void LayerStack::SetAllActive(bool state)
	{
		for (auto& layer : m_layers)
		{
			layer->setActive(state);
		}
	}

}
