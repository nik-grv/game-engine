/** \file OpenGLShader.cpp*/

#include "engine_pch.h"
#include "platform/OpenGL/OpenGLShader.h"
#include <fstream>
#include "systems/log.h"
#include <string>
#include <array>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
namespace Engine
{
	OpenGLShader::OpenGLShader(const char* vertexFilePath, const char* fragmentFilePath)
	{
		std::string line, vertexSrc, fragSrc;
		
		//read vertex shader
		std::fstream handle(vertexFilePath, std::ios::in);
		if (handle.is_open())
		{
			while (getline(handle,line))
				vertexSrc += line + "\n";
		}
		else
		{
			Log::error("Could not open the Vertex file path {0} :", vertexFilePath);
			return; 
		}
		handle.close();

		//read frag shader
		handle.open(fragmentFilePath, std::ios::in);
		if (handle.is_open())
		{
			while (getline(handle, line))
				fragSrc += line + "\n";
		}
		else
		{
			Log::error("Could not open the Fragment file path {0} :", fragmentFilePath);
			return;
		}
		handle.close();

		CompileAndLink(vertexSrc.c_str(), fragSrc.c_str());
	}

	OpenGLShader::OpenGLShader(const char* filePath)
	{
		enum Region {
			None = -1,Vertex = 0, Fragment, Geometry, TesselationControl, TesselationEvaluation, Compute };
		std::string line;
		std::array<std::string, Region::Compute + 1> src;
		uint32_t region = Region::None;

		//read vertex shader
		std::fstream handle(filePath, std::ios::in);
		if (handle.is_open())
		{
			while (getline(handle, line))
			{
				if (line.find("#region Vertex") != std::string::npos) { region = Region::Vertex; continue; }
				if (line.find("#region Fragment") != std::string::npos) { region = Region::Fragment; continue; }
				if (line.find("#region Geometry") != std::string::npos) { region = Region::Geometry; continue; }
				if (line.find("#region TesselationControl") != std::string::npos) { region = Region::TesselationControl; continue; }
				if (line.find("#region TesselationEvaluation") != std::string::npos) { region = Region::TesselationEvaluation; continue; }
				if (line.find("#region Compute") != std::string::npos) { region = Region::Compute; continue; }

				if (region != Region::None)
					src[region] += line + "\n";
			}
		}
		else
		{
			Log::error("Could not open the file path {0} :", filePath);
			return;
		}
		handle.close();

		CompileAndLink(src[Region::Vertex].c_str(), src[Region::Fragment].c_str());
	}


	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_OpenGL_ID);	
	}

	void OpenGLShader::uploadInt(const char* name, const int value)
	{
		uint32_t uniformLocation = glGetUniformLocation(m_OpenGL_ID, name);
		glUniform1i(uniformLocation, value);
	}

	void OpenGLShader::uploadFloat(const char* name, const float value)
	{
		uint32_t uniformLocation = glGetUniformLocation(m_OpenGL_ID, name);
		glUniform1f(uniformLocation, value);
	}

	void OpenGLShader::uploadFloat2(const char* name, const glm::vec2& value)
	{
		uint32_t uniformLocation = glGetUniformLocation(m_OpenGL_ID, name);
		glUniform2f(uniformLocation, value.x,value.y);
	}

	void OpenGLShader::uploadFloat3(const char* name, const glm::vec3& value)
	{
		uint32_t uniformLocation = glGetUniformLocation(m_OpenGL_ID, name);
		glUniform3f(uniformLocation, value.x,value.y,value.z);
	}

	void OpenGLShader::uploadFloat4(const char* name, const glm::vec4& value)
	{
		uint32_t uniformLocation = glGetUniformLocation(m_OpenGL_ID, name);
		glUniform4f(uniformLocation, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::uploadMat4(const char* name,const glm::mat4& value)
	{
		uint32_t uniformLocation = glGetUniformLocation(m_OpenGL_ID, name);
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::uploadIntArray(const char* name, int32_t* values, uint32_t count)
	{
		uint32_t uniformLocation = glGetUniformLocation(m_OpenGL_ID, name);
		glUniform1iv(uniformLocation,count,values);
	}

	void OpenGLShader::CompileAndLink(const char* vertexShaderSrc, const char* fragmentShaderSrc)
	{
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const GLchar* source = vertexShaderSrc;
		glShaderSource(vertexShader, 1, &source, 0);
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
			Log::error("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteShader(vertexShader);
			return;
		}

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		source = fragmentShaderSrc;
		glShaderSource(fragmentShader, 1, &source, 0);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
			Log::error("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			return;
		}

		m_OpenGL_ID = glCreateProgram();
		glAttachShader(m_OpenGL_ID, vertexShader);
		glAttachShader(m_OpenGL_ID, fragmentShader);
		glLinkProgram(m_OpenGL_ID);

		GLint isLinked = 0;
		glGetProgramiv(m_OpenGL_ID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_OpenGL_ID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_OpenGL_ID, maxLength, &maxLength, &infoLog[0]);
			Log::error("Shader linking error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteProgram(m_OpenGL_ID);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			return;
		}

		glDetachShader(m_OpenGL_ID, vertexShader);
		glDetachShader(m_OpenGL_ID, fragmentShader);


	}
}