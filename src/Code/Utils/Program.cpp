#include <glad/glad.h>
#include <iostream>
#include "ProgramShader.h"

namespace ProgramShader {
	Program::Program()
	{
		ProgramId = 0;
		shaderIds = nullptr;
		shaderCount = 0;
	}

	Program::~Program()
	{
		for (int i = 0; i < shaderCount; i++)
		{
			glDetachShader(ProgramId, shaderIds[i]);
			glDeleteShader(shaderIds[i]);
		}
		glDeleteProgram(ProgramId);
		delete[] shaderIds;
		shaderIds = nullptr;
	}

	unsigned int Program::genShader(const std::string& shaderDir, int shaderType)
	{
		unsigned int shaderId;

		std::string shaderString{};
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
			return 0;
		}

		const char* shaderCode = shaderString.c_str();	//Get the shader code as a c-style string

		shaderId = glCreateShader(shaderType);
		glShaderSource(shaderId, 1, &shaderCode, NULL);
		glCompileShader(shaderId);

		int success = {};
		char infoLog[1024];
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderId, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR: " << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;;
			std::cout << shaderCode;
			std::cout << "\n -- --------------------------------------------------- -- " << std::endl;
			return 0;
		}
		return shaderId;
	}

	bool Program::attachShaders()
	{
		for (int i = 0; i < shaderCount; i++)
		{
			glAttachShader(ProgramId, shaderIds[i]);
		}
		glLinkProgram(ProgramId);

		int success = {};
		char infoLog[1024] = {};
		glGetProgramiv(ProgramId, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ProgramId, 1024, NULL, infoLog);
			std::cout << "Error linking shader code: " << '\n' << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			return false;
		}
		return true;
	}

	void Program::init(const std::string& vertDir,const std::string& fragDir)
	{
		if (ProgramId != 0)
		{
			std::cout << "Program already exists";
			return;
		}

		shaderCount = 2;

		ProgramId = glCreateProgram();	//Initilize program and array of shaders
		shaderIds = new unsigned int[2]{};

		shaderIds[0] = genShader(vertDir, GL_VERTEX_SHADER);
		shaderIds[1] = genShader(fragDir, GL_FRAGMENT_SHADER);
		
		attachShaders();
	}

	void Program::use()
	{
		glUseProgram(ProgramId);
	}

	void Program::setBool(const char* name, bool value) { glUniform1i(glGetUniformLocation(ProgramId,name),value);}
	void Program::setInt(const char* name, int value) { glUniform1i(glGetUniformLocation(ProgramId,name),value);}
	void Program::setFloat(const char* name, float value) { glUniform1f(glGetUniformLocation(ProgramId,name),value);}
	void Program::setVec2(const char* name, glm::vec2 value) { glUniform2f(glGetUniformLocation(ProgramId, name),value.x,value.y); }
	void Program::setVec3(const char* name, glm::vec3 value) { glUniform3f(glGetUniformLocation(ProgramId, name),value.x,value.y,value.z); }
	void Program::setVec4(const char* name, glm::vec4 value) { glUniform4f(glGetUniformLocation(ProgramId, name),value.x,value.y,value.z,value.w); }
	void Program::setMat4(const char* name, glm::mat4 value){glUniformMatrix4fv(glGetUniformLocation(ProgramId,name),1,GL_FALSE,&value[0][0]);}
}