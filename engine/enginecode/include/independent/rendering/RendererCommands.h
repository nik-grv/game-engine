/** \file RendererCommands.h*/
#pragma once

#include <functional>


namespace Engine {


	class RendererShared;
	/**
	\class RendererCommands
	* \brief class to include render commands
	*/
	class RendererCommands
	{
	public:
		/** \enum Commands
		* \brief Enum for Renderer commands
		*/
		enum class Commands  { clearColorAndDepthBuffer,setClearColor ,enableCommand,disableCommand , blendFuncCommand , useProgramCommand , 
			setLineMode , setFillMode
		};
	private:
		std::function<void(void)> m_action; //!< action of the render commans
		friend class RenderCommandFactory; //!< make RenderCommandFactory a friend class
		friend class RendererShared;	//!< make RendererShared a friend class
	};

	/**
	\class RenderCommandFactory
	* \brief class with the template function to create RendererCommands via tuple
	*/
	class RenderCommandFactory
	{
	public:
		template<typename ...Args> static RendererCommands* createCommand(RendererCommands::Commands command, Args&& ...args) //!< create render command 
		{
			RendererCommands* result = new RendererCommands;
			auto argTuple = std::make_tuple(args...);
			switch (command)
			{
			case Engine::RendererCommands::Commands::clearColorAndDepthBuffer:
				result->m_action = getClearColorAndDepthBuffer();
				return result;
			case Engine::RendererCommands::Commands::setLineMode:
				result->m_action = setGlLine();
				return result;
			case Engine::RendererCommands::Commands::setFillMode:
				result->m_action = setGlFill();
				return result;
			case Engine::RendererCommands::Commands::enableCommand:
				uint32_t enableCmd;
				getValue<uint32_t, 0>(enableCmd, argTuple);
				result->m_action = glEnableCmd(enableCmd);
				return result;
			case Engine::RendererCommands::Commands::disableCommand:
				uint32_t disableCmd;
				getValue<uint32_t, 0>(disableCmd, argTuple);
				result->m_action = glDisableCmd(disableCmd);
				return result;
			case Engine::RendererCommands::Commands::setClearColor:
				float r, g, b, a;

				getValue<float, 0>(r, argTuple);
				getValue<float, 1>(g, argTuple);
				getValue<float, 2>(b, argTuple);
				getValue<float, 3>(a, argTuple);

				result->m_action = getSetClearCommand(r, g, b, a);
				return result;
			case Engine::RendererCommands::Commands::blendFuncCommand:
				uint32_t sFactor, dFactor;
				getValue<uint32_t, 0>(sFactor, argTuple);
				getValue<uint32_t, 1>(dFactor, argTuple);
				result->m_action = glBlendFuncCmd(sFactor, dFactor);
				return result;
			case Engine::RendererCommands::Commands::useProgramCommand:
				uint32_t program;
				getValue<uint32_t, 0>(program, argTuple);
				result->m_action = glUseProgramCmd(program);
				return result;
			}
		}

	private:
		static std::function<void(void)> getClearColorAndDepthBuffer(); //!< function to clear the color and depth buffer
		static std::function<void(void)> getSetClearCommand(float r, float  g, float b, float  a); //!< function to set clear color 
		static std::function<void(void)> glEnableCmd(uint32_t cmd); //!< function to enable gl commands
		static std::function<void(void)> glDisableCmd(uint32_t cmd); //!< function to disable gl commands
		static std::function<void(void)> glBlendFuncCmd(uint32_t sFactor , uint32_t dFactor); //!< function to blend gl commands
		static std::function<void(void)> glUseProgramCmd(uint32_t program); //!< function to use program
		static std::function<void(void)> setGlLine(); //!< function to draw gl lines
		static std::function<void(void)> setGlFill(); //!< function to draw gl fill

		template <typename G, size_t I, typename... Ts>
		typename std::enable_if<I >= sizeof...(Ts),void>::type
		static getValue(G& result, std::tuple<Ts...> tup)
		{
			//if index is > or = tuple size, dont do anything..
		}

		template <typename G, size_t I, typename... Ts>
		typename std::enable_if<(I < sizeof...(Ts)),void>::type
		static  getValue(G& result, std::tuple<Ts...> tup)
		{
			//get the value at I position in tuple
			result = std::get<I>(tup);
		}
	};

}
