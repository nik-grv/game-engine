/** \file ShaderRend.h*/
#pragma once

#include <glm/glm.hpp>

namespace Engine {

	/**
	\class ShaderRend
	*\brief API Agnostic class to include OpenGLShader
	*/
	class ShaderRend
	{
	public:
		virtual ~ShaderRend() = default; //!< destructor
		virtual inline uint32_t getRenderID() const = 0; //!< get render iD
		virtual void uploadInt(const char* name, const int value) = 0;			//!< set int in shader file
		virtual void uploadFloat(const char* name, const float value) = 0;		//!< set float in shader file
		virtual void uploadFloat2(const char* name, const glm::vec2& value) = 0;	//!< set vec2 in shader file
		virtual void uploadFloat3(const char* name, const glm::vec3& value) = 0;	//!< set vec3 in shader file
		virtual void uploadFloat4(const char* name, const glm::vec4& value) = 0;	//!< set vec4 in shader file
		virtual void uploadMat4(const char* name, const glm::mat4& value) = 0;	//!< set mat4 in shader file
		virtual void uploadIntArray(const char* name, int32_t* values,uint32_t count) = 0;	//!< set int array in shader file
		virtual void CompileAndLink(const char* vertexShaderSrc, const char* fragmentShaderSrc) = 0; //!< complie the shader files

		static ShaderRend* create(const char* vertexFilePath, const char* fragmentFilePath); //!< constructor with vertex and frag shader file path params
		static ShaderRend* create(const char* filePath); //!< constructor
	};
}