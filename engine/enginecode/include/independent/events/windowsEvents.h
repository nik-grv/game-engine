/** \file windowsEvents.h*/
#pragma once

#include "event.h"
#include <glm/glm.hpp>

namespace Engine
{
	/** \class WindowCloseEvent
	*\brief interface to handle the window close event*/
	class WindowCloseEvent : public Event
	{
	public:
		static EventTypes getStaticType() { return EventTypes::WindowClose; } //!< return static type of window close event
		virtual inline EventTypes getEventType() const override { return EventTypes::WindowClose; } //!< Get event type
		virtual int32_t getCategoryFlags() const override { return EventCategory::EventCategoryWindow; } //!< Get event category flags
	};
	
	/** \class WindowFocusEvent
	*\brief interface to handle the window focus event*/
	class WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent() {}; //!< empty constructor
		static EventTypes getStaticType() { return EventTypes::WindowFocus; } //!< return static type of window focus event
		virtual inline EventTypes getEventType() const override { return EventTypes::WindowFocus; } //!< Get event type
		virtual int32_t getCategoryFlags() const override { return EventCategory::EventCategoryWindow; } //!< Get event category flags
	};
	
	/** \class WindowLostFocusEvent
	*\brief interface to handle the window lost focus event*/
	class WindowLostFocusEvent : public Event
	{
	public:
		WindowLostFocusEvent() {};  //!< empty constructor
		static EventTypes getStaticType() { return EventTypes::WindowLostFocus; } //!< return static type of window lost focus event
		virtual inline EventTypes getEventType() const override { return EventTypes::WindowLostFocus; } //!< Get event type
		virtual int32_t getCategoryFlags() const override { return EventCategory::EventCategoryWindow; } //!< Get event category flags
	};
	
	/** \class WindowResizeEvent
	*\brief interface to handle the window resize event */
	class WindowResizeEvent : public Event
	{
	public:
		//! Constructor which sets the window size
		/*!
		\param width int32_t - resized window width
		\param height int32_t - resized window height
		*/
		WindowResizeEvent(int32_t w, int32_t h) : m_width(w),m_height(h) {}
		static EventTypes getStaticType() { return EventTypes::WindowResize; } //!< return static type of window close event
		virtual inline EventTypes getEventType() const override { return EventTypes::WindowResize; } //!< Get event type
		virtual int32_t getCategoryFlags() const override { return EventCategory::EventCategoryWindow; } //!< Get event category flags

		inline int32_t getWidth() const { return m_width; } //!< get resized window width
		inline int32_t getHeight() const { return m_height; } //!< get resized window height
		inline glm::ivec2 getSize() const { return { m_width,m_height }; } //!< get resized window in vec2
	private:
		int32_t m_width;	//!<width after window resize
		int32_t m_height;	//!<height after window resize
	};
	
	/** \class WindowMovedEvent
	*\brief interface to handle the window resize event */
	class WindowMovedEvent : public Event
	{
	public:
		//! Constructor which sets the window position on screen
		/*!
		\param xPosition int32_t - X position of window
		\param yPosition int32_t - Y position of window
		*/
		WindowMovedEvent(int32_t x, int32_t y) : m_newPosX(x),m_newPosY(y) {}
		static EventTypes getStaticType() { return EventTypes::WindowMoved; } //!< return static type of window moved event
		virtual inline EventTypes getEventType() const override { return EventTypes::WindowMoved; } //!< Get event type
		virtual int32_t getCategoryFlags() const override { return EventCategory::EventCategoryWindow; } //!< Get event category flags

		inline int32_t getXPosition() const { return m_newPosX; } //!< get window x position
		inline int32_t getYPosition() const { return m_newPosY; } //!< get window y Position
		inline glm::ivec2 getPos() const { return { m_newPosX,m_newPosY}; } //!< get moved window x,y in vec2
	private:
		int32_t m_newPosX;	//!<width after window resize
		int32_t m_newPosY;	//!<height after window resize
	};
}