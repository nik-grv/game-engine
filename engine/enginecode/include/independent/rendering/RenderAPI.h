/** \file RenderAPI.h
*/
#pragma once

namespace Engine {

	/**
	\class RenderAPI
	*\brief class to switch render API at runtime
	*/
	class RenderAPI
	{
	public:
		/** \enum API
		* \brief Enum for event types
		*/
		enum class API{ None = 0, OpenGL=1, Direct3d = 2, Vulkan = 3};
		inline static API getAPI() { return s_API; }	//!< get render api
	private:
		static API s_API;	//!< render api
		
	};

}