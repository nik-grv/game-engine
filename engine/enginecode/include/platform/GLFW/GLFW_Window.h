/** \file GLFW_Window.h
*/
#pragma once


#include "core/window.h"
#include <GLFW/glfw3.h>

namespace Engine {
	/** \struct GLFW_Window
	*\brief implementation of window using GLFW. Inherits from Window class
	*/
	class GLFW_Window : public Window
	{
	public:
		GLFW_Window(const WindowProperties& properties);//!< constructor for this class
		virtual void init(const WindowProperties& properties) override;//!< initialise the window
		virtual void close() override;//!< close the window 
		//virtual ~Window() {}; //!< destructor for window class
		virtual void onUpdate(float timestep) override; //!< update the window
		virtual void setvSync(bool vSync) override; //!<set vsync for the window
		virtual inline unsigned int getWidth() const override {return m_properties.width; } //!< get window width
		virtual inline unsigned int getHeight() const override { return m_properties.height; }//!< get window height
		virtual inline void* getNativewindow() const override { return m_native; }//!<get window platform glfw/win32
		virtual inline bool isFullScreenMode() const override { return m_properties.isFullScreen; }//!<get window fullscreen status
		virtual inline bool isvSync() const override { return m_properties.isVsyncOn; } //!< get window vSync status

	private:
		WindowProperties m_properties;//!<window properties
		GLFWwindow* m_native; //!< pointer to native GLFWwindow 
		float m_aspectRatio; //!< aspect ratio
	};
}