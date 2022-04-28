/*\file widgets.h*/

#pragma once

#include <glm/glm.hpp>

#include "UI/UIComponent.h"

namespace Engine
{
	class Widget : public UIComponent //!< Widget Component, with update of position and size
	{
	public:
		virtual void updatePosition(glm::ivec2& relativePosition) override;
		virtual void updateSize() override;
	};

	class Spacer : public Widget //!< Widget that creates Space
	{
	public:
		Spacer(glm::ivec2 size);
		Spacer(int32_t sizeX, int32_t sizeY);
		virtual void onRender() const override {};
	};

	class Label : public Widget //!< Quad with Text but Quad doesn't get rendered 
	{
	public:
		Label(glm::ivec2 size, const char* labelText);
		Label(int32_t sizeX, int32_t sizeY, const char* labelText);
		virtual void onRender() const;
		virtual void updatePosition(glm::ivec2& relativePosition) override;
	protected:
		const char* m_text;
		glm:: ivec2 m_textPosition;
	};

	class Button : public Label //!< Label that's clickable 
	{
	public: 
		Button(int32_t x, int32_t y, const char* buttonText, std::function<void(void)> onClick) :
			Label(x,y,buttonText),
			m_callback(onClick)
		{}
		Button(glm::ivec2 size, const char* buttonText, std::function<void(void)> onClick) :
			Label(size, buttonText),
			m_callback(onClick)
		{}
		virtual void onRender() const;
		virtual void onMouseBtnPressed(glm::ivec2& mousePosition, int32_t button) override;
	private:
		std::function<void(void)> m_callback;
	};
	
	class Slider : public Widget
	{
	public: 
		Slider(int32_t width, float low, float high, const char* lowText, const char* highText);
		virtual void updatePosition(glm::ivec2& relativePosition) override;
		virtual void onRender() const override;
		virtual void onMouseMoved(glm::ivec2& mousePosition) override; 
		virtual void onMouseBtnPressed(glm::ivec2& mousePosition, int32_t button) override;
		virtual void onMouseBtnReleased(glm::ivec2& mousePosition, int32_t button) override;
		float getValue() { return m_currentvalue; }
	private:
		const char* m_lowText;
		const char* m_highText;
		char m_valueText[128];
		float m_sliderWidth;
		float m_higherBound;
		float m_lowerBound;
		float m_currentvalue;
		float m_sliderHeight = 12;
		glm::ivec2 m_relativePosition;
		glm::ivec2 m_sliderPosition;
		glm::ivec2 m_grabPosition;
		glm::ivec2 m_lowTextPosition;
		glm::ivec2 m_highTextPosition;
		glm::ivec2 m_grabTextPosition;
		bool m_grabActive = false;
	};
}