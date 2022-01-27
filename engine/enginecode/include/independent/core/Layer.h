/** \file Layer.h*/
#pragma once

#include "events/events.h"

namespace Engine
{
	class Layer
	{
	public:
		Layer(const char* name) : m_layerName(name) {};
		virtual void OnAttach() {}
		virtual void OnDettach() {}
		virtual void OnUpdate(float timeStep) {}
		virtual void OnRender() {}
		virtual void OnResize(WindowResizeEvent& event) {}

		virtual void onWindowClose(WindowCloseEvent& e) { e.handle(true); } //!<run when window closes
		virtual void onWindowResize(WindowResizeEvent&e) { e.handle(true); } //!<run when resizes
		virtual void onWindowMoved(WindowMovedEvent&e) { e.handle(true); } //!<run when window is moved
		virtual void onWindowFocus(WindowFocusEvent&e) { e.handle(true); } //!<run when window has focus
		virtual void onWindowLostFocus(WindowLostFocusEvent&e) { e.handle(true); } //!<run when window loses focus
		
		virtual void onKeyPressed(KeyPressedEvent&e) { e.handle(true); } //!<run when keyboard button is pressed
		virtual void onKeyReleased(KeyReleasedEvent&e) { e.handle(true); } //!<run when keyboard button is released
		
		virtual void onMouseMoved(MouseMovedEvent&e) { e.handle(true); } //!<run when mouse is moved
		virtual void onMouseBtnPressed(MouseButtonPressedEvent&e) { e.handle(true); } //!<run when mouse button is pressed
		virtual void onMouseBtnReleased(MouseButtonReleasedEvent&e) { e.handle(true); } //!<run when mouse button is released
		virtual void onMouseScrolled(MouseScrollEvent&e) { e.handle(true); } //!<run when mouse button is released
		

		inline const char* getLayerName() const{ return m_layerName; }
		inline void setActive(bool state) { m_isActive = state; }
		inline const bool isActive() { return m_isActive; }


	private:
		const char* m_layerName;
		bool m_isActive = true;
	};
}