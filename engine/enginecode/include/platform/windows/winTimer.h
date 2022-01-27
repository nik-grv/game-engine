/** \file winTimer.h*/
#pragma once

#include <Windows.h>
#include "core/timer.h"

namespace Engine
{
	/** \Class WinTimer
	*\brief Windows Timer to use QueryPerformanceCount to get high resolution time-intervals. Inherits from Timer class
	*/
	class WinTimer : public Timer
	{
	public:
		virtual void start() override //!< Start winTimer
		{
			QueryPerformanceFrequency(&m_frequency);
			QueryPerformanceCounter(&m_startTime);
		}; 
		virtual inline void reset() override //!< Reset winTimer
		{
			QueryPerformanceCounter(&m_startTime);
		}; 
		virtual float getElapsedTime() override
		{
			QueryPerformanceCounter(&m_endTime);
			float resultTime = (m_endTime.QuadPart - m_startTime.QuadPart) * 1000.0 / m_frequency.QuadPart;
			resultTime /= 1000.0f;
			return resultTime;
		}
	private:
		LARGE_INTEGER m_startTime; //!< Start time for the winTimer
		LARGE_INTEGER m_endTime;  //!< End time for the winTimer
		LARGE_INTEGER m_frequency;  //!< Ticks per second for the CPU
	};
		
}

