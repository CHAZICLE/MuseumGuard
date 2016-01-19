#include <iostream>
#include "util/Console.hpp"

#include "ShaderUtils.hpp"

//#define __SHADER_DEBUG

namespace shaders {
	GLuint loadShaders(const char *programName, const char *vertexCode, const char *fragmentCode)
	{
		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		
		GLint compileStatus;
		GLint infoLogLength;
		
		// Compile vertex shader
		glShaderSource(vertexShaderID, 1, &vertexCode, 0);
		glCompileShader(vertexShaderID);
		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &compileStatus);
		glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(compileStatus==GL_FALSE)
		{
			char infoLog[infoLogLength+1];
			glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, infoLog);
			std::cerr << "Failed to compile vertex shader: " << programName << std::endl << "Error Log [#" << (infoLogLength-1) << "]: " << infoLog << std::endl << "Vertcode:" << std::endl << vertexCode << std::endl;
			std::exit(1);
		}
		
		// Compile fragment shader
		glShaderSource(fragmentShaderID, 1, &fragmentCode, 0);
		glCompileShader(fragmentShaderID);
		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &compileStatus);
		glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(compileStatus==GL_FALSE)
		{
			char infoLog[infoLogLength+1];
			glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, infoLog);
			infoLog[infoLogLength] = 0;
			std::cerr << "Failed to compile fragment shader: " << programName << "\n" << infoLog << std::endl;
			std::exit(1);
		}
		
		// Link program
		GLuint programID = glCreateProgram();
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		glLinkProgram(programID);
		
		glGetShaderiv(programID, GL_LINK_STATUS, &compileStatus);
		glGetShaderiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(compileStatus==GL_FALSE)
		{
			char infoLog[infoLogLength+1];
			glGetShaderInfoLog(programID, infoLogLength, NULL, infoLog);
			infoLog[infoLogLength] = 0;
			std::cerr << "Failed to compile fragment shader: " << programName << "\n" << infoLog << std::endl;
			std::exit(1);
		}
		
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		
		//PRINTLN(TAG_SHADERS, "Compile program " << programName << ":" << programID);
		return programID;
	}
	GLint getUniformLocation(const GLuint programID, const char *uniformName)
	{
		GLint g = glGetUniformLocation(programID, uniformName);
		//PRINTLN(TAG_SHADERS, "\tUniform variable [" << programID << "]" << uniformName << ":" << g);
		return g;
	}
	GLint getAttributeLocation(const GLuint programID, const char *attributeName)
	{
		GLint g = glGetAttribLocation(programID, attributeName);
		//PRINTLN(TAG_SHADERS, "\tAttribute variable [" << programID << "]" << attributeName << ":" << g);
		return g;
	}
#ifndef SKIP_DEPEND_TREE
#include "shadercode.c"
#endif
}
