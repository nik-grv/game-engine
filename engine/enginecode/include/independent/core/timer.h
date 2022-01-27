/** \file timer.h*/

#pragma once

#include <chrono>

namespace Engine
{
	/** \class Timer
	* \brief interface for timer class
	*/

	class Timer
	{
	public:
		virtual void start() = 0; //!< Start the timer
		virtual  void reset() = 0;//!< Reset timer
		virtual float getElapsedTime() = 0; //!< Get elapsed time since last restart
	};

	/** \class ChronoTimer
	* \brief the chrono timer class to calculate elapsed time. Inherits from the timer class
	*/

	class ChronoTimer : public Timer
	{
	public:
		virtual inline void start() override { m_startTimerPoint = std::chrono::high_resolution_clock::now(); } //!< Start chrono timer
		virtual inline void reset() override { m_startTimerPoint = std::chrono::high_resolution_clock::now(); }//!< Reset chrono timer
		virtual float getElapsedTime() override 
		{
			m_endTimerPoint = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float, std::milli> elapsed = m_endTimerPoint - m_startTimerPoint;
			return elapsed.count() / 1000.0f;
		} //!< Calculate elapsed time

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimerPoint; //!< Start time for the chronoTimer
		std::chrono::time_point<std::chrono::high_resolution_clock> m_endTimerPoint;  //!< End time for the chronoTimer
	};
}