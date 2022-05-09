/** \file log.h
*/

#pragma once

#include "system.h"
#include <spdlog/spdlog.h>

namespace Engine
{
	/**
	\class Log
	*\brief template abstract class to Log info in the console
	*/
	class Log : public System
	{
	public:
		virtual void start(SystemSignal init = SystemSignal::None, ...) override; //!< Start the logger
		virtual void stop(SystemSignal close = SystemSignal::None, ...) override; //!< Stop the logger
#pragma region [ Program Function Declare ]
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

		template <class ...Args>
		static void e_info(Args&&... args); //!< static method for logging info

		template <class ...Args>
		static void e_warn(Args&&... args); //!< static method for logging warning

		template <class ...Args>
		static void e_trace(Args&&... args);//!< static method for logging trace

		template <class ...Args>
		static void e_debug(Args&&... args);//!< static method for debug

		template <class ...Args>
		static void e_error(Args&&... args);//!< static method for logging error

		template <class ...Args>
		static void e_release(Args&&... args);//!< static method for release	

		template <class ...Args>
		static void s_info(Args&&... args); //!< static method for logging info

		template <class ...Args>
		static void s_warn(Args&&... args); //!< static method for logging warning

		template <class ...Args>
		static void s_trace(Args&&... args);//!< static method for logging trace

		template <class ...Args>
		static void s_debug(Args&&... args);//!< static method for debug

		template <class ...Args>
		static void s_error(Args&&... args);//!< static method for logging error

		template <class ...Args>
		static void s_release(Args&&... args);//!< static method for release	
#pragma endregion
		inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return s_myLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetEditorLogger() { return s_Editor; }
		inline static std::shared_ptr<spdlog::logger>& GetSandboxLogger() { return s_Sandbox; }
		inline static std::shared_ptr<spdlog::logger>& GetFileLogger() { return s_myFileLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_myLogger; //!< Engine logger
		static std::shared_ptr<spdlog::logger> s_Editor; //!< Editor logger
		static std::shared_ptr<spdlog::logger> s_Sandbox; //!< File logger
		static std::shared_ptr<spdlog::logger> s_myFileLogger; //!< File logger

	};

#pragma region [ Program Logger Functions ]

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

	template<class ...Args>
	void Log::e_info(Args && ...args)
	{
#ifdef NG_DEBUG
		s_Editor->info(std::forward<Args>(args) ...);
#endif
	}

	template<class ...Args>
	void Log::e_warn(Args && ...args)
	{
#ifdef NG_DEBUG
		s_Editor->warn(std::forward<Args>(args) ...);
#endif
	}

	template<class ...Args>
	void Log::e_trace(Args && ...args)
	{
#ifdef NG_DEBUG
		s_Editor->trace(std::forward<Args>(args) ...);
#endif
	}

	template<class ...Args>
	void Log::e_debug(Args && ...args)
	{
#ifdef NG_DEBUG
		s_Editor->debug(std::forward<Args>(args) ...);
#endif
	}

	template<class ...Args>
	void Log::e_error(Args && ...args)
	{
#ifdef NG_DEBUG
		s_Editor->error(std::forward<Args>(args) ...);
#endif
	}

	template<class ...Args>
	void Log::e_release(Args && ...args)
	{
		s_Editor->trace(std::forward<Args>(args) ...);
	}

	template<class ...Args>
	void Log::s_info(Args && ...args)
	{
#ifdef NG_DEBUG
		s_Sandbox->info(std::forward<Args>(args) ...);
#endif
	}

	template<class ...Args>
	void Log::s_warn(Args && ...args)
	{
#ifdef NG_DEBUG
		s_Sandbox->warn(std::forward<Args>(args) ...);
#endif
	}

	template<class ...Args>
	void Log::s_trace(Args && ...args)
	{
#ifdef NG_DEBUG
		s_Sandbox->trace(std::forward<Args>(args) ...);
#endif
	}

	template<class ...Args>
	void Log::s_debug(Args && ...args)
	{
#ifdef NG_DEBUG
		s_Sandbox->debug(std::forward<Args>(args) ...);
#endif
	}

	template<class ...Args>
	void Log::s_error(Args && ...args)
	{
#ifdef NG_DEBUG
		s_Sandbox->error(std::forward<Args>(args) ...);
#endif
	}

	template<class ...Args>
	void Log::s_release(Args && ...args)
	{
		s_Sandbox->trace(std::forward<Args>(args) ...);
	}
#pragma endregion
}
#pragma region [ Macros ]
//Engine Log Macros
#define ENGINE_INFO(...)		::Engine::Log::GetEngineLogger()->info(__VA_ARGS__)
#define ENGINE_WARN(...)		::Engine::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define ENGINE_TRACE(...)		::Engine::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define ENGINE_DEBUG(...)		::Engine::Log::GetEngineLogger()->debug(__VA_ARGS__)
#define ENGINE_ERROR(...)		::Engine::Log::GetEngineLogger()->error(__VA_ARGS__)
#define ENGINE_RELEASE(...)		::Engine::Log::GetEngineLogger()->release(__VA_ARGS__)

//Editor Log Macros
#define EDITOR_INFO(...)		::Engine::Log::GetEditorLogger()->info(__VA_ARGS__)
#define EDITOR_WARN(...)		::Engine::Log::GetEditorLogger()->warn(__VA_ARGS__)
#define EDITOR_TRACE(...)		::Engine::Log::GetEditorLogger()->trace(__VA_ARGS__)
#define EDITOR_DEBUG(...)		::Engine::Log::GetEditorLogger()->debug(__VA_ARGS__)
#define EDITOR_ERROR(...)		::Engine::Log::GetEditorLogger()->error(__VA_ARGS__)
#define EDITOR_RELEASE(...)		::Engine::Log::GetEditorLogger()->release(__VA_ARGS__)

//Editor Log Macros
#define SANDBOX_INFO(...)		::Engine::Log::GetSandboxLogger()->info(__VA_ARGS__)
#define SANDBOX_WARN(...)		::Engine::Log::GetSandboxLogger()->warn(__VA_ARGS__)
#define SANDBOX_TRACE(...)		::Engine::Log::GetSandboxLogger()->trace(__VA_ARGS__)
#define SANDBOX_DEBUG(...)		::Engine::Log::GetSandboxLogger()->debug(__VA_ARGS__)
#define SANDBOX_ERROR(...)		::Engine::Log::GetSandboxLogger()->error(__VA_ARGS__)
#define SANDBOX_RELEASE(...)		::Engine::Log::GetSandboxLogger()->release(__VA_ARGS__)
#pragma endregion