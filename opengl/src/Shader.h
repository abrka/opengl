#pragma once
#include <string>
#include "glad/glad.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Texture.h"
#include "CubemapTexture.h"

class GlShaderProgram {

public:
	unsigned int ID{ 0 };

	void Bind() const{
		glUseProgram(ID);
	}
	void Unbind() const {
		glUseProgram(0);
	}

	GlShaderProgram(const std::string& FragPath, const std::string& VertexPath) {

		std::ifstream VertexFile{ VertexPath };

		if (not VertexFile.is_open()) {
			std::cout << "couldnt open vertex file from shader program " <<ID<< "the path was "<< VertexPath;
			return;
		}

		std::stringstream VertexStream;
		VertexStream << VertexFile.rdbuf();
		std::string VertexSrc = VertexStream.str();
		const char* VertexSrcCStr = VertexSrc.c_str();

		unsigned int VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(VertexShaderID, 1, &VertexSrcCStr, NULL);
		glCompileShader(VertexShaderID);
		PrintShaderCompileErrors(VertexShaderID);


		std::ifstream FragFile{ FragPath };

		if (not VertexFile.is_open()) {
			std::cout << "couldnt open fragment file from shader program " << ID << "the path was " << FragPath;
			return;
		}

		std::stringstream FragStream;
		FragStream << FragFile.rdbuf();
		std::string FragSrc = FragStream.str();
		const char* FragSrcCStr = FragSrc.c_str();

		unsigned int FragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(FragShaderID, 1, &FragSrcCStr, NULL);
		glCompileShader(FragShaderID);
		PrintShaderCompileErrors(FragShaderID);


		ID = glCreateProgram();
		glAttachShader(ID, VertexShaderID);
		glAttachShader(ID, FragShaderID);
		glLinkProgram(ID);
		PrintShaderProgramLinkErrors(ID);
		glDeleteShader(VertexShaderID);
		glDeleteShader(FragShaderID);
	}

	static void PrintShaderCompileErrors(unsigned int ShaderID) {
		int  success;
		char infoLog[512];
		glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(ShaderID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}


	static void PrintShaderProgramLinkErrors(unsigned int ShaderProgramID){
		int  success;
		char infoLog[512];
		glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ShaderProgramID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::LINKER ERROR\n" << infoLog << std::endl;
		}
	}

	

	void ValidateUniform(const std::string& name) const{
		/*if (glGetUniformLocation(ID, name.c_str()) == -1) {
			std::cout << "the uniform youre setting: " << name << " does not exist/it is the wrong type/ it is not used by all shaders/it was discarded by the shader compiler because it is unused\n";
		}*/
	}
	void SetFloat(const std::string& name, const float value) const {
		
		ValidateUniform(name);
		glUniform1f( glGetUniformLocation(ID, name.c_str()), value);
	}
	void SetVec3(const std::string& name, const glm::vec3 value) const {

		ValidateUniform(name);
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
	}
	void SetInt(const std::string& name, const int value)const {

		ValidateUniform(name);
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void SetMatrix4f(const std::string& name, const glm::mat4& value, bool transpose = false) const {

		ValidateUniform(name);
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, transpose, glm::value_ptr(value));
		
	}
	void SetTexture(const std::string& name, const GlTexture& Tex, unsigned int TextureUnit) const {
		Tex.Activate(TextureUnit);
		SetInt(name, TextureUnit);
	
	}
	void SetCubemapTexture(const std::string& name, const GlCubemapTexture& Tex, unsigned int TextureUnit) const {
		Tex.Activate(TextureUnit);
		SetInt(name, TextureUnit);
		
	}


	GlShaderProgram(const GlShaderProgram& rhs) = delete;
	GlShaderProgram& operator=(const GlShaderProgram& rhs) = delete;
};