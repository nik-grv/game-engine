/** \file keyboardEvents.h*/
#pragma once

#include "event.h"
#include <glm/glm.hpp>

namespace Engine
{
	/** \class KeyEvent
	*\brief base class for keyboard events*/
	class KeyEvent : public Event
	{
	protected:
		//! Constructor which sets the keycode
		/*!
		\param keyCode int32_t - key press code
		*/
		KeyEvent(int32_t keycode) : m_keyCode(keycode) {} 
		int32_t m_keyCode; //!< key code for the key pressed
	public:
		inline int32_t getKeyCode() const { return m_keyCode; } //!< get the key code
		virtual inline int32_t getCategoryFlags() const override { return EventCategoryKeyboard | EventCategoryInput; } //!< get event category
	};

	/** \class KeyPressedEvent
	*\brief interface to handle the key pressed event*/
	class KeyPressedEvent : public KeyEvent
	{
	public:
		//! Constructor which sets the keycode and repeat cound
		/*!
		\param keyCode int32_t - key press code
		\param keyRepeatCount int32_t - key repeated press count
		*/
		KeyPressedEvent(int32_t  keyPressedCode, int32_t repeatCount) : KeyEvent(keyPressedCode), m_repeatCount(repeatCount) {}
		
		int32_t getRepeatCount() const { return m_repeatCount; } //!< get repeat count for key
		static EventTypes getStaticType() { return EventTypes::KeyPressed; } //!< return static type of key pressed event
		virtual inline EventTypes getEventType() const override { return getStaticType(); } //!< Get event type
		int32_t m_repeatCount; //!< key repeat count
	};

	/** \class KeyReleasedEvent
	*\brief interface to handle the key released event*/
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		//! Constructor which sets the keycode
		/*!
		\param keyCode int32_t - key released code
		*/
		KeyReleasedEvent(int32_t keyReleasedCode) : KeyEvent(keyReleasedCode) {}

		static EventTypes getStaticType() { return EventTypes::KeyReleased; } //!< return static type of key released event
		virtual inline EventTypes getEventType() const override { return getStaticType(); } //!< Get event type
	};
	
	/** \class KeyTypedEvent
	*\brief interface to handle the key pressed event*/
	class KeyTypedEvent : public KeyEvent
	{
	public:
		//! Constructor which sets the keycode
		/*!
		\param keyCode int32_t - key typed code
		*/
		KeyTypedEvent(int32_t keyTypedCode): KeyEvent (keyTypedCode){}

		static EventTypes getStaticType() { return EventTypes::KeyTyped; } //!< return static type of key typed event
		virtual inline EventTypes getEventType() const override { getStaticType(); } //!< Get event type
	};

}