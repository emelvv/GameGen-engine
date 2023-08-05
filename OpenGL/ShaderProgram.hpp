#pragma once

// https://habr.com/ru/articles/313380/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
// #define DEBUG


namespace Renderer {
	class ShaderProgram {
	public:
		GLuint Program;

		ShaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath);

		void SetMat4(glm::mat4, const char*);
		void Use();
	private:
		void CheckShaderCompileError(GLuint shader);
	};


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

#ifdef DEBUG
			std::cout << vertexCode << std::endl;
			std::cout << fragmentCode << std::endl;
#endif

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

	void ShaderProgram::SetMat4(glm::mat4 mat, const char* name) {
		glUniformMatrix4fv(glGetUniformLocation(this->Program, name), 1, GL_FALSE, glm::value_ptr(mat));
	}


	void ShaderProgram::Use() {
		glUseProgram(this->Program);
	}
}