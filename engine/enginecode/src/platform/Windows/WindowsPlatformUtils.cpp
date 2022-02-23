#include "engine_pch.h"
#include "utilities/PlatformUtils.h"

//#include <commdlg.h>
//#include <GLFW/glfw3.h>
//#define GLFW_EXPOSE_NATIVE_WIN32
//#include <GLFW/glfw3native.h>

#include "core/application.h"

namespace Engine {

	std::string FileDialogs::OpenFile(const char* filter)
	{
		std::string d = ".test";
		Log::e_debug("Attempted to Open File: *.tanky");
		return d;
	}

	std::string FileDialogs::SaveFile(const char* filter)
	{
		std::string d = ".test";
		Log::e_debug("Attempted to Save File: *.tanky");
		return d;
	}


}
