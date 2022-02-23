#pragma once


namespace Engine
{
	class LabelComponent
	{
	public:
		LabelComponent(const char* label) : m_label(label) {}
		const char* m_label;
	};
}