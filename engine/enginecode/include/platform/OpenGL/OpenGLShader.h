/** \file OpenGLShader.h*/
#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include "rendering/ShaderRend.h"

namespace Engine {

	/**
	\class OpenGLShader
	*\brief class to read vertex and fragmenr shader
	*/
	class OpenGLShader : public ShaderRend
	{
	public:
		//! Overloaded constructor for OpenGLShader class
		/*!
		\param path char - vertex shader file path
		\param path char - fragment shader file path
		*/
		OpenGLShader(const char* vertexFilePath, const char* fragmentFilePath); //!< constructor with vertex and frag shader file path params
		OpenGLShader(const char* filePath); //!< constructor
		virtual ~OpenGLShader(); //!< destructor
		virtual inline uint32_t getRenderID() const override{ return m_OpenGL_ID; } //!< get render iD
		virtual void uploadInt(const char* name, const  int value)override;			//!< set int in shader file
		virtual void uploadFloat(const char* name, const float value)override;		//!< set float in shader file
		virtual void uploadFloat2(const char* name, const glm::vec2& value)override;	//!< set vec2 in shader file
		virtual void uploadFloat3(const char* name, const glm::vec3& value)override;	//!< set vec3 in shader file
		virtual void uploadFloat4(const char* name, const glm::vec4& value)override;	//!< set vec4 in shader file
		virtual void uploadMat4(const char* name, const glm::mat4& value)override;	//!< set mat4 in shader file
		virtual void uploadIntArray(const char* name, int32_t* values, uint32_t count) override; //!< set int array in shader file
	private:
		uint32_t m_OpenGL_ID;	//!< render id
		virtual void CompileAndLink(const char* vertexShaderSrc, const char* fragmentShaderSrc) override; //!< complie the shader files
	};
}