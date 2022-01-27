/** \file system.h*/
#pragma once

#include <cstdarg>

namespace Engine {

	/** \enum SystemSignal
	* \brief Enum SystemSignal flags
	*/
	enum class SystemSignal { None = 0 };

	/**
	\class System
	*\brief virtual class to provide interface for the system
	*/
	class System
	{
	public:
		virtual ~System() {};
		virtual void start(SystemSignal init = SystemSignal::None, ...) = 0; //!< Start the system
		virtual void stop(SystemSignal close = SystemSignal::None, ...) = 0; //!< Stop the system
	};
}
