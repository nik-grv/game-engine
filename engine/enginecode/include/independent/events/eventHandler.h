/** \file eventHandler.h*/
#pragma once

#include "events.h"
#include <functional>
namespace Engine
{
	/** \class EventHandler
	* \brief class for handling events*/

	class EventHandler
	{
	public:
		//window event callbacks
		void setOnCloseCallback(const std::function<bool(WindowCloseEvent&)>& fn) { m_onCloseCallbackFn = fn; } //!< window close accessor function
		void setOnResizeCallback(const std::function<bool(WindowResizeEvent&)>& fn) { m_onResizeCallbackFn = fn; } //!< accessor function
		void setOnMovedCallback(const std::function<bool(WindowMovedEvent&)>& fn) { m_onMovedCallbackFn = fn; } //!< accessor function
		void setOnFocusCallback(const std::function<bool(WindowFocusEvent&)>& fn) { m_onFocusCallbackFn = fn; } //!< accessor function
		void setOnLostFocusCallback(const std::function<bool(WindowLostFocusEvent&)>& fn) { m_onLostFocusCallbackFn = fn; } //!< accessor function
		std::function<bool(WindowCloseEvent&)>& getOnCloseCallback() { return m_onCloseCallbackFn; } //!< getter function to return the callback
		std::function<bool(WindowResizeEvent&)>& getOnResizeCallback() { return m_onResizeCallbackFn; } //!<  getter function to return the callback
		std::function<bool(WindowMovedEvent&)>& getOnMovedCallback() { return m_onMovedCallbackFn; } //!<  getter function to return the callback
		std::function<bool(WindowLostFocusEvent&)>& getOnLostFocusCallback() { return m_onLostFocusCallbackFn; } //!<  getter function to return the callback
		std::function<bool(WindowFocusEvent&)>& getOnFocusCallback() { return m_onFocusCallbackFn; } //!<  getter function to return the callback

		//keyboard event callbacks
		void setOnKeyEvent(const std::function<bool(KeyEvent&)>& fn) { m_onKeyEventCallbackFn = fn; } //!< accessor function
		void setOnKeyPressedEvent(const std::function<bool(KeyPressedEvent&)>& fn) { m_onKeyPressedEventCallbackFn = fn; } //!< accessor function
		void setOnKeyReleasedEvent(const std::function<bool(KeyReleasedEvent&)>& fn) { m_onKeyReleasedEventCallbackFn = fn; } //!< accessor function
		void setOnKeyTypedEvent(const std::function<bool(KeyTypedEvent&)>& fn) { m_onKeyTypedEventCallbackFn = fn; } //!< accessor function
		std::function<bool(KeyEvent&)>& getOnKeyEventCallback() { return m_onKeyEventCallbackFn; } //!<  getter function to return the callback
		std::function<bool(KeyPressedEvent&)>& getOnKeyPressedEventCallback() { return m_onKeyPressedEventCallbackFn; } //!<  getter function to return the callback
		std::function<bool(KeyReleasedEvent&)>& getOnKeyReleasedEventCallback() { return m_onKeyReleasedEventCallbackFn; } //!<  getter function to return the callback
		std::function<bool(KeyTypedEvent&)>& getOnKeyTypedEventCallback() { return m_onKeyTypedEventCallbackFn; } //!<  getter function to return the callback

		//mouse event callbacks
		void setOnMouseMovedEvent(const std::function<bool(MouseMovedEvent&)>& fn) { m_onMouseMovedEventCallbackFn = fn; } //!< accessor function
		void setOnMouseScrolledEvent(const std::function<bool(MouseScrollEvent&)>& fn) { m_onMouseScrolledEventCallbackFn = fn; } //!< accessor function
		void setOnMouseBtnPressedEvent(const std::function<bool(MouseButtonPressedEvent&)>& fn) { m_onMouseBtnPressedEventCallbackFn = fn; } //!< accessor function
		void setOnMouseBtnReleasedEvent(const std::function<bool(MouseButtonReleasedEvent&)>& fn) { m_onMouseBtnReleasedEventCallbackFn = fn; } //!< accessor function
		std::function<bool(MouseMovedEvent&)>& getOnMouseMovedEventCallback() { return m_onMouseMovedEventCallbackFn; } //!<  getter function to return the callback
		std::function<bool(MouseScrollEvent&)>& getOnMouseScrolledEventCallback() { return m_onMouseScrolledEventCallbackFn; } //!<  getter function to return the callback
		std::function<bool(MouseButtonPressedEvent&)>& getOnMouseBtnPressedEventCallback() { return m_onMouseBtnPressedEventCallbackFn; } //!<  getter function to return the callback
		std::function<bool(MouseButtonReleasedEvent&)>& getOnMouseBtnReleasedEventCallback() { return m_onMouseBtnReleasedEventCallbackFn; } //!<  getter function to return the callback


	private:
		//window fn
		std::function<bool(WindowCloseEvent&)> m_onCloseCallbackFn = std::bind(&EventHandler::defaultOnCloseCallback, this, std::placeholders::_1); //!<bind the function to make it callable
		std::function<bool(WindowResizeEvent&)> m_onResizeCallbackFn = std::bind(&EventHandler::defaultOnResizeCallback , this, std::placeholders::_1); //!<bind the function to make it callable 
		std::function<bool(WindowMovedEvent&)> m_onMovedCallbackFn = std::bind(&EventHandler::defaultOnMovedCallback , this, std::placeholders::_1); //!<bind the function to make it callable 
		std::function<bool(WindowLostFocusEvent&)> m_onLostFocusCallbackFn = std::bind(&EventHandler::defaultOnLostFocusCallback , this, std::placeholders::_1); //!<bind the function to make it callable 
		std::function<bool(WindowFocusEvent&)> m_onFocusCallbackFn = std::bind(&EventHandler::defaultOnFocusCallback , this, std::placeholders::_1); //!<bind the function to make it callable 
		
		//window bool callbacks
		bool defaultOnCloseCallback(WindowCloseEvent& e) { return false; } //!< return false when not event handled 
		bool defaultOnResizeCallback(WindowResizeEvent& e) { return false; } //!< return false when not event handled 
		bool defaultOnMovedCallback(WindowMovedEvent& e) { return false; } //!< return false when not event handled 
		bool defaultOnLostFocusCallback(WindowLostFocusEvent& e) { return false; } //!< return false when not event handled 
		bool defaultOnFocusCallback(WindowFocusEvent& e) { return false; } //!< return false when not event handled 

		//keyboard fn
		std::function<bool(KeyEvent&)> m_onKeyEventCallbackFn = std::bind(&EventHandler::defaultOnKeyEventCallback , this, std::placeholders::_1); //!<bind the function to make it callable 
		std::function<bool(KeyPressedEvent&)> m_onKeyPressedEventCallbackFn = std::bind(&EventHandler::defaultOnKeyPressedEventCallback , this, std::placeholders::_1); //!<bind the function to make it callable 
		std::function<bool(KeyReleasedEvent&)> m_onKeyReleasedEventCallbackFn = std::bind(&EventHandler::defaultOnKeyReleasedEventCallback , this, std::placeholders::_1); //!<bind the function to make it callable 
		std::function<bool(KeyTypedEvent&)> m_onKeyTypedEventCallbackFn = std::bind(&EventHandler::defaultOnKeyTypedEventCallback , this, std::placeholders::_1); //!<bind the function to make it callable 
		//keyboard bool callbacks 
		bool defaultOnKeyEventCallback(KeyEvent& e) { return false; } //!< return false when not event handled 
		bool defaultOnKeyPressedEventCallback(KeyPressedEvent& e) { return false; } //!< return false when not event handled 
		bool defaultOnKeyReleasedEventCallback(KeyReleasedEvent& e) { return false; } //!< return false when not event handled 
		bool defaultOnKeyTypedEventCallback(KeyTypedEvent& e) { return false; } //!< return false when not event handled

		//mouse fn
		std::function<bool(MouseMovedEvent&)> m_onMouseMovedEventCallbackFn = std::bind(&EventHandler::defaultOnMouseMovedEventCallback , this, std::placeholders::_1); //!<bind the function to make it callable 
		std::function<bool(MouseScrollEvent&)> m_onMouseScrolledEventCallbackFn = std::bind(&EventHandler::defaultOnMouseScrolledEventCallback , this, std::placeholders::_1); //!<bind the function to make it callable 
		std::function<bool(MouseButtonPressedEvent&)> m_onMouseBtnPressedEventCallbackFn = std::bind(&EventHandler::defaultOnMouseBtnPressedEventCallback , this, std::placeholders::_1); //!<bind the function to make it callable 
		std::function<bool(MouseButtonReleasedEvent&)> m_onMouseBtnReleasedEventCallbackFn = std::bind(&EventHandler::defaultOnMouseBtnReleasedEventCallback , this, std::placeholders::_1); //!<bind the function to make it callable 
		//mouse bool callbacks
		bool defaultOnMouseMovedEventCallback(MouseMovedEvent& e) { return false; } //!< return false when not event handled
		bool defaultOnMouseScrolledEventCallback(MouseScrollEvent& e) { return false; } //!< return false when not event handled
		bool defaultOnMouseBtnPressedEventCallback(MouseButtonPressedEvent& e) { return false; } //!< return false when not event handled
		bool defaultOnMouseBtnReleasedEventCallback(MouseButtonReleasedEvent& e) { return false; } //!< return false when not event handled

	};
}