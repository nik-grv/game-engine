/** \file mouseEvents.h*/
#pragma once

#include "event.h"
#include <glm/glm.hpp>

namespace Engine
{
	/** \class MouseEvent
	*\brief base class for mouse events*/
	class MouseEvent : public Event
	{
	protected:
		int32_t m_button; //!< button code
		//! Constructor which sets the pressed mouse button code
		/*!
		\param buttonCode int32_t - button press code
		*/
		void MouseBtnSet(int32_t buttonCode) { m_button=buttonCode; }
	public:
		inline int32_t getButton() const { return m_button; } //!< get the button code
		virtual inline int32_t getCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; } //<! get category flags
	};

	/** \class MouseButtonPressedEvent
	*\brief interface to handle the mouse button press event*/
	class MouseButtonPressedEvent : public MouseEvent
	{
	public:
		//! Constructor which sets the pressed mouse button code
		/*!
		\param buttonCode int32_t - button press code
		*/
		MouseButtonPressedEvent(int32_t  mouseBtn)  {MouseBtnSet(mouseBtn); } //!< constructor to set mouse button pressed
		static EventTypes getStaticType() { return EventTypes::MouseBtnPressed; } //!< return static type of key pressed event
		virtual inline EventTypes getEventType() const override { return getStaticType(); } //!< Get event type
	};
	
	/** \class MouseButtonReleasedEvent
	*\brief interface to handle the mouse button released event*/
	class MouseButtonReleasedEvent : public MouseEvent
	{
	public:
		//! Constructor which sets the released mouse button code
		/*!
		\param buttonCode int32_t - button press code
		*/
		MouseButtonReleasedEvent(int32_t  mouseBtn) { MouseBtnSet(mouseBtn); } //!< constructor to set mouse button released
		static EventTypes getStaticType() { return EventTypes::MouseBtnReleased; } //!< return static type of key pressed event
		virtual inline EventTypes getEventType() const override { return getStaticType(); } //!< Get event type
	};
	
	/** \class MouseMovedEvent
	*\brief interface to handle the mouse moved event*/
	class MouseMovedEvent : public MouseEvent
	{
	public:
		//! Constructor which sets the mouse position
		/*!
		\param XPosition float - Position of mouse in X axis
		\param YPosition float - Position of mouse in Y axis
		*/
		MouseMovedEvent(float x, float y) : m_xPos(x), m_yPos(y) {} //!< constructor to set mouse moved position
		static EventTypes getStaticType() { return EventTypes::MouseMoved; } //!< return static type of key pressed event
		virtual inline EventTypes getEventType() const override { return getStaticType(); } //!< Get event type
		inline float getX() const { return m_xPos; } //!< get X position of mouse
		inline float getY() const { return m_yPos; } //!< gte Y position of mouse
		inline glm::vec2 getMousePos() const { return glm::vec2(m_xPos,m_yPos); } //!< get XY position of mouse as a Vec2
	private:
		float m_xPos; //!< mouse movement in x axis
		float m_yPos;	//!< mouse movement in y axis
	};

	/** \class MouseScrollEvent
	*\brief interface to handle the mouse scroll event*/
	class MouseScrollEvent : public MouseEvent
	{
	public:
		//! Constructor which sets the mouse scroll
		/*!
		\param deltaX float - Vertical scroll
		\param deltaY float - Horizontal scroll
		*/
		MouseScrollEvent(float deltaX, float deltaY) : m_deltaX(deltaX), m_deltaY(deltaY) {} //!< constructor to set mouse scroll 
		static EventTypes getStaticType() { return EventTypes::MouseScrolled; } //!< return static type of key pressed event
		virtual inline EventTypes getEventType() const override { return getStaticType(); } //!< Get event type
		inline float getXScroll() const { return m_deltaX; } //!< get X scroll
		inline float getYScroll() const { return m_deltaY; } //!< get Y scroll
	private:
		float m_deltaX; //!< scroll in x 
		float m_deltaY;	//!< scroll in y
	};
}