/** \file log.cpp*/

#include "engine_pch.h"
#include "systems/log.h"


namespace Engine
{
	/**
	* Creating a basic logger Header file
	* that contains a shared pointer to a static spdlog logger
	*/
	std::shared_ptr<spdlog::logger> Log::s_myLogger = nullptr;
	std::shared_ptr<spdlog::logger> Log::s_myFileLogger = nullptr;

	void Engine::Log::start(SystemSignal init, ...)
	{
		spdlog::set_pattern("%^[%T]: %v%$");
		spdlog::set_level(spdlog::level::trace);

		s_myLogger = spdlog::stdout_color_mt("Console");

		char filePath[256] = "logs/";
		char time[128];

		/*
		try
		{
			std::time_t t = std::time(nullptr);
			std::strftime(time, sizeof(time), "%d_%m_%y-%I_%M_%S", std::localtime(&t));

			strcat_s(filePath, time);
			strcat_s(filePath, ".txt");
			s_myFileLogger = spdlog::basic_logger_mt("File", filePath);
		}
		catch (const spdlog::spdlog_ex& e)
		{
			s_myLogger->error("Cannot start file logger - {0]", e.what());
			s_myFileLogger.reset();
		}
		*/
	}

	void Engine::Log::stop(SystemSignal close, ...)
	{
		s_myLogger->info("Stopping logger");
		s_myLogger.reset();
	}

}