#pragma once

#include <glad/glad.h>
#include <glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>


namespace ProgramShader {
	class Program 
	{
	private:
		unsigned int ProgramId;
		unsigned int* shaderIds;
		int shaderCount;
		unsigned int genShader(const std::string& shaderDir, int shaderType);
		bool attachShaders();
	public:
		Program();
		~Program();
		void init(const std::string& vertDir, const std::string& fragDir);
		void use();

		void setBool(const char* name, bool value);
		void setInt(const char* name, int value);
		void setFloat(const char* name, float value);
		void setVec2(const char* name, glm::vec2 value);
		void setVec3(const char* name, glm::vec3 value);
		void setVec4(const char* name, glm::vec4 value);
		void setMat4(const char* name, glm::mat4 value);
	};
}