workspace "Engine"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}"

group "Engine"

project "Engine"
	location "engine"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/" .. outputdir .. "/%{prj.name}")
	
	pchheader "engine_pch.h"
	pchsource "engine/precompiled/engine_pch.cpp"
	
	files
	{
		"%{prj.name}/enginecode/**.h",
		"%{prj.name}/enginecode/**.cpp",
		"engine/precompiled/engine_pch.h",
		"engine/precompiled/engine_pch.cpp"
	}

	includedirs
	{
		"%{prj.name}/enginecode/",
		"%{prj.name}/enginecode/include/independent",
		"%{prj.name}/enginecode/include/",
		"%{prj.name}/precompiled/",
		"vendor/spdlog/include",
		"vendor/glfw/include",
		"vendor/Glad/include",
		"vendor/glm/",
		"vendor/STBimage",
		"vendor/freetype2/include",
		"vendor/json/single_include/nlohmann",
		"vendor/IMGui",
		"vendor/enTT/single_include",
		"vendor/luaBridge/Source",
		"vendor/assimp/include",
		"vendor/box2d/include",
		"vendor/lua",
		"vendor/react3d/include"
	}
	
	links 
	{
		"GLFW",
		"Glad",
		"Freetype",
		"IMGui",
		"assimp",
		"box2d",
		"lua",
		"ReactPhysics3d"
		
	}
	
	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"NG_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "NG_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "NG_RELEASE"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/include",
		"engine/enginecode/",
		"engine/enginecode/include/independent",
		"engine/enginecode/include/",
		"engine/precompiled/",
		"vendor/glfw/include",
		"vendor/glm/",
		"vendor/spdlog/include",
		"vendor/json/single_include/nlohmann",
		"vendor/freetype2/include",
		"vendor/IMGui",
		"vendor/enTT/single_include",
		"vendor/luaBridge/Source",
		"vendor/assimp/include",
		"vendor/box2d/include",
		"vendor/lua",
		"vendor/react3d/include"
	}

	links
	{
		"Engine"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"NG_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "NG_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "NG_RELEASE"
		runtime "Release"
		optimize "On"



project "Engine-Editor"
		location "Engine-Editor"
		kind "ConsoleApp"
		language "C++"
		staticruntime "off"
	
		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("build/" .. outputdir .. "/%{prj.name}")
	
		files
		{
			"%{prj.name}/editorcode/include/**.h",
			"%{prj.name}/editorcode/src/**.cpp",
			"%{prj.name}/editorcode/OpenGL/**.cpp"
		}
	
		includedirs
		{
			"%{prj.name}/include",
			"engine/enginecode/",
			"engine/enginecode/include/independent",
			"engine/enginecode/include/",
			"engine/precompiled/",
			"vendor/glfw/include",
			"vendor/glm/",
			"vendor/glad/include",
			"vendor/spdlog/include",
			"vendor/json/single_include/nlohmann",
			"vendor/freetype2/include",
			"vendor/IMGui",
			"vendor/enTT/single_include",
			"vendor/luaBridge/Source",
			"vendor/assimp/include",
			"vendor/box2d/include",
			"vendor/lua",
			"vendor/react3d/include"
		}
	
		links
		{
			"Engine",
			"IMGui",
			"Glad"
		}
	
		filter "system:windows"
			cppdialect "C++17"
			systemversion "latest"
	
			defines
			{
				"NG_PLATFORM_WINDOWS"
			}
	
		filter "configurations:Debug"
			defines "NG_DEBUG"
			runtime "Debug"
			symbols "On"
	
		filter "configurations:Release"
			defines "NG_RELEASE"
			runtime "Release"
			optimize "On"

 project "EngineTests"
		location "engineTests"
        kind "ConsoleApp"
        language "C++"
		staticruntime "off"
		systemversion "latest"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("build/" .. outputdir .. "/%{prj.name}")

        files 
		{ 
			"%{prj.name}/include/*.h",
			"%{prj.name}/src/*.cpp"
		}

        includedirs
		{ 
			"%{prj.name}/include/",
			"vendor/googletest/googletest/include",
			"engine/enginecode/",
			"engine/enginecode/include/independent",
			"engine/enginecode/include/platform",
			"engine/precompiled/",
			"vendor/spdlog/include",
			"vendor/glfw/include",
			"vendor/Glad/include",
			"vendor/glm/",
			"vendor/STBimage",
			"vendor/freetype2/include",
			"vendor/enTT/single_include",
			"vendor/luaBridge/Source",
			"vendor/assimp/include",
			"vendor/box2d/include",
			"vendor/lua",
			"vendor/react3d/include"
			
		}

        links 
		{ 
			"googletest",
			"Engine"
		}
		
		filter "configurations:Debug"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			runtime "Release"
			optimize "On"

project "Spike"
	location "spike"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"engine/enginecode/",
		"engine/enginecode/include/independent",
		"engine/enginecode/include/platform",
		"engine/precompiled/",
		"%{prj.name}/include",
		"vendor/spdlog/include",
		"vendor/STBimage",
		"vendor/freetype2/include",
		"vendor/glm/",
		"vendor/Glad/include",
		"vendor/glfw/include",
		"vendor/json/single_include/nlohmann",
		"vendor/enTT/single_include",
		"vendor/luaBridge/Source",
		"vendor/assimp/include",
		"vendor/box2d/include",
		"vendor/lua",
		"vendor/react3d/include"
	}
	
	links 
	{
		"Engine",
		"Freetype",
		"Glad",
		"GLFW",
		"IMGui"
	}
	

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		defines
		{
			"NG_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"

group "Vendor"



	include "vendor/glfw"
	include "vendor/googletest"
	include "vendor/Glad"
	include "vendor/freetype2"
	include "vendor/IMGui"
	include "vendor/assimp"
	include "vendor/box2d"
	include "vendor/lua"
	include "vendor/react3d"

