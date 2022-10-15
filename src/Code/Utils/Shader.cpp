#include <glad/glad.h>
#include "ProgramShader.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace ProgramShader {

	Shader::Shader()
	{
		ID=0;
	}

	Shader::Shader(const std::string& shaderDir, int shaderType) {
		std::string shaderString = {};

		std::ifstream shaderFile;
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			shaderFile.open(shaderDir);		//Opens file
			std::stringstream shaderStream;

			shaderStream << shaderFile.rdbuf();	//Reads file into stream
			shaderFile.close();

			shaderString = shaderStream.str();	//Converts stream into std::string

		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
		}

		const char* shaderCode = shaderString.c_str();

		ID = glCreateShader(shaderType);
		glShaderSource(ID, 1, &shaderCode, NULL);
		glCompileShader(ID);

		int success = {};
		char infoLog[1024];
		glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(ID, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR: " << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;;
			std::cout << shaderCode;
			std::cout << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}

	Shader::~Shader()
	{
		glDeleteShader(ID);
	}
	
	unsigned int Shader::getID() { return ID; }
}