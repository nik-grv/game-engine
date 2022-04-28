/*\file UIComponent.h */

#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	struct UIColourScheme
	{
		glm::vec4 background = { 0.2f, 0.2f,0.2f,1.f }; //!< background colour
		glm::vec4 foreground = { 1.f, 1.f,1.f,1.f }; //!< foreground colour 
		glm::vec4 highlight = { 0.4f, 0.2f,0.6f,1.f }; //!< highlight colour 
	};

	class UIComponent
	{
	public:
		glm::ivec2 getPosition() { return m_position; } //!< gets the position 
		glm::ivec2 getSize() { return m_size; } //!< gets the size 
		
		virtual void updateSize() = 0; //!< updates the size 
		virtual void updatePosition(glm::ivec2& relativePosition) = 0; //!< updates position with relative position 
		virtual void onRender() const {}; //!< renders it 
		virtual void onMouseMoved(glm::ivec2& mousePosition) {}; //!< responds to the movement of the mouse 
		virtual void onMouseBtnPressed(glm::ivec2& mousePosition, int32_t button) {}; //!< responds to the movement of the mouse button pressed
		virtual void onMouseBtnReleased(glm::ivec2& mousePosition, int32_t button) {}; //!< responds to the movement of the mouse button released
		void setparent(UIComponent* parent) { m_parent = parent; } //!< sets the parent 
		UIComponent* getParent() const { return m_parent; } //!< gets the parent 
		UIColourScheme* m_colourScheme = nullptr; //!< colour scheme 

	protected:
		glm::ivec2 m_position;
		glm::ivec2 m_size;
		UIComponent* m_parent = nullptr;
	};
}