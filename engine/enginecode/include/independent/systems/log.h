/** \file log.h
*/

#pragma once

#include "system.h"
#include <spdlog/spdlog.h>

namespace Engine
{
	/**
	\class Log
	*\brief templated abstract class to Log info in the console
	*/
	class Log : public System
	{
	public:
		virtual void start(SystemSignal init = SystemSignal::None, ...) override; //!< Start the logger
		virtual void stop(SystemSignal close = SystemSignal::None, ...) override; //!< Stop the logger

		template <class ...Args>
		static void info(Args&&... args); //!< static method for logging info

		template <class ...Args>
		static void warn(Args&&... args); //!< static method for logging warning

		template <class ...Args>
		static void trace(Args&&... args);//!< static method for logging trace

		template <class ...Args>
		static void debug(Args&&... args);//!< static method for debug

		template <class ...Args>
		static void error(Args&&... args);//!< static method for logging error
		
		template <class ...Args>
		static void release(Args&&... args);//!< static method for release	
		
		template <class ...Args>
		static void file(Args&&... args);//!< static method for file logging

	private:
		static std::shared_ptr<spdlog::logger> s_myLogger; //!< Console logger
		static std::shared_ptr<spdlog::logger> s_myFileLogger; //!< File logger

	};


	template<class ...Args>
	void Log::info(Args && ...args)
	{
#ifdef NG_DEBUG
		s_myLogger->info(std::forward<Args>(args) ...);
#endif
	}

	template<class ...Args>
	void Log::warn(Args && ...args)
	{
#ifdef NG_DEBUG
		s_myLogger->warn(std::forward<Args>(args) ...);
#endif
	}

	template<class ...Args>
	void Log::trace(Args && ...args)
	{
#ifdef NG_DEBUG
		s_myLogger->trace(std::forward<Args>(args) ...);
#endif
	}

	template<class ...Args>
	void Log::debug(Args && ...args)
	{
#ifdef NG_DEBUG
		s_myLogger->debug(std::forward<Args>(args) ...);
#endif
	}

	template<class ...Args>
	void Log::error(Args && ...args)
	{
#ifdef NG_DEBUG
		s_myLogger->error(std::forward<Args>(args) ...);
#endif
	}

	template<class ...Args>
	void Log::release(Args && ...args)
	{
		s_myLogger->trace(std::forward<Args>(args) ...);
	}

	template<class ...Args>
	void Log::file(Args && ...args)
	{
		if(s_myFileLogger)
		s_myFileLogger->trace(std::forward<Args>(args) ...);
	}
}