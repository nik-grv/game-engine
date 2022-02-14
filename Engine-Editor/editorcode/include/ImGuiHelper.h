#pragma once
//#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui.h>
//#include "ImGuiGLFW.h";		// e ../editorcode/OpenGL/ImGuiGLFW.h";// <examples/imgui_impl_glfw.h>
//#include "ImGuiOpenGL.h";	//"../editorcode/OpenGL/ImGuiOpenGL.h";// <examples/imgui_impl_opengl3.h>
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

namespace Engine
{
	class ImGuiHelper
	{
	public:
		static void init();
		static void begin();
		static void end();
		static void shutdown();
	};
}