/** \file window.h
*/
#pragma once


#include "events/eventHandler.h"
#include "core/graphicsContext.h"

namespace Engine {
	/** \struct WindowProperties
	*\brief common properties for windows defined when creating a GLFW window
	*/

	/** \struct WindowPropeties
	*\brief common properties for windows defined when creating a GLFW window
	\param title char - title of the window
	\param width uint32_t - width of the window
	\param height uint32_t - height of the window
	\param fullscreen bool - is the window in fullscreen
	*/
	struct WindowProperties
	{
		char* title;		//!< window title
		uint32_t width;		//!< width of window
		uint32_t height;	//!< height of window
		bool isFullScreen;	//!< is window fullscreen
		bool isVsyncOn;		//!< is Vsync on

		//! Function to set the default window properties 
		/*!
		\param title char - title of the window
		\param width uint32_t - width of the window
		\param height uint32_t - height of the window
		\param fullscreen bool - is the window in fullscreen
		*/
		WindowProperties(char* title = "My GLFW Window", uint32_t w = 800, uint32_t h = 600, bool fullscreen = false) :
			title(title), width(w), height(h), isFullScreen(fullscreen) {} //!< define default window properties
	};

	/** \class Window
	* \brief abstract interface base class for GLFW window
	*/
	class Window
	{
	public:
		virtual void init(const WindowProperties& properties) = 0;//!< initialise the window
		virtual void close() = 0;//!< close the window 
		virtual ~Window() {}; //!< destructor for window class
		virtual void onUpdate(float timestep) = 0; //!< update the window
		virtual void setvSync(bool vSync) = 0; //!<set vsync for the window
		virtual unsigned int getWidth() const = 0; //!< get window width
		virtual unsigned int getHeight() const = 0;//!< get window height
		virtual void* getNativewindow() const = 0;//!<get window platform glfw/win32
		virtual bool isFullScreenMode() const = 0;//!<get window fullscreen status
		virtual bool isvSync() const = 0; //!< get window vSync status
		inline EventHandler& getEventHandler() { return m_eventHandler; }//!< get event handler
		static Window* create(const WindowProperties& properties = WindowProperties()); //!<create window with the properties

	protected:
		EventHandler m_eventHandler; //!< event handler
		std::shared_ptr<GraphicsContext> m_graphicsContext;//!<graphics context
	};

}