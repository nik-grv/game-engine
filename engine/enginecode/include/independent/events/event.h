/** \file event.h*/
#pragma once

#include <stdint.h>

namespace Engine
{
	/** \enum EventTypes
	* \brief Enum for event types
	*/

	enum class EventTypes
	{
		None = 0, //!< none
		WindowClose, //!< window close event
		WindowResize, //!< window resize event
		WindowFocus,//!< window focus event
		WindowLostFocus, //!< window lost focus event
		WindowMoved,//!< window moved event
		KeyPressed, //!< key pressed event
		KeyReleased, //!< key released event
		KeyTyped,//!< key typed event
		MouseBtnPressed, //!< mouse button pressed event
		MouseBtnReleased, //!< mouse button released
		MouseMoved,	//!< mouse moved event
		MouseScrolled	//!< mouse scrolled event
	};

	/** \enum EventCategory
	* Enum for type categories*/

	enum EventCategory
	{
		None = 0,
		EventCategoryWindow = 1 << 0,		//!< 0000 0001 - 1
		EventCategoryInput = 1 << 1,		//!< 0000 0010 - 2
		EventCategoryKeyboard = 1 << 2,		//!< 0000 0100 - 3
		EventCategoryMouse = 1 << 3,		//!< 0000 1000 - 4
		EventCategoryMouseButton = 1 << 4	//!< 0001 0000 - 5
	};
	 
	/** \class Event
	* \brief Base class for the Events*/

	class Event
	{
	public:
		virtual EventTypes getEventType() const = 0; //!< Get event type
		virtual int32_t getCategoryFlags() const = 0; //!< Get event category flags
		inline bool isHandled() const { return m_handled; } //!< is the event handled
		inline void handle(bool handle) { m_handled = handle; } //!< handle the event
		inline bool isInEventCategory(EventCategory eventCat) const { return getCategoryFlags() & eventCat; } //!< is the event in the category
	protected:
		bool m_handled = false; //!< has the event handled

	};

}