#pragma once
#include <string>
#include "glad/glad.h"
#include <iostream>
#include <fstream>
#include <sstream>

class GlShaderProgram {

public:
	unsigned int ID;

	void Use() {
		glUseProgram(ID);
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


	static void PrintShaderProgramLinkErrors(unsigned int ShaderProgramID) {
		int  success;
		char infoLog[512];
		glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ShaderProgramID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::LINKER ERROR\n" << infoLog << std::endl;
		}
	}
};