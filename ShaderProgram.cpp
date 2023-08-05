#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


namespace Renderer {
	void ShaderProgram::CheckShaderCompileError(GLuint shader) {
		int  success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	ShaderProgram::ShaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath) {
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile(vertexPath);
		std::ifstream fShaderFile(fragmentPath);

		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);

		try
		{
			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();
			
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (const std::exception* ex)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode = fragmentCode.c_str();


		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);


		glShaderSource(vs, 1, &vShaderCode, nullptr);
		glCompileShader(vs);
		this->CheckShaderCompileError(vs);

		glShaderSource(fs, 1, &fShaderCode, nullptr);
		glCompileShader(fs);
		this->CheckShaderCompileError(fs);

		this->Program = glCreateProgram();
		glAttachShader(this->Program, vs);
		glAttachShader(this->Program, fs);
		glLinkProgram(this->Program);

		glDeleteShader(vs);
		glDeleteShader(fs);

	}


	void ShaderProgram::Use() {
		glUseProgram(this->Program);
	}

}
