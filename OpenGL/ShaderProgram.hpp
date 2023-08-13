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


namespace Engine {
	class ShaderProgram {
	public:
		GLuint Program;

		ShaderProgram();
		ShaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath);

		void SetMat4(glm::mat4, const char*);
		void Set1i(int newInt, const char* name);
		void Use();

		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram& operator=(ShaderProgram&&) = delete;
	private:
		void CheckShaderCompileError(GLuint shader);
		void CreateShaderProgram(const char* vertexShader, const char* fragmentShader);

		const GLchar* defaultVertexShader =
		 R"(#version 450
			layout(location = 0) in vec3 vertex_position;
			layout(location = 1) in vec2 tex_position;
			out vec2 TexCoord;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;

			void main() {
				gl_Position = projection * view * model * vec4(vertex_position, 1.0);
				TexCoord = tex_position;
			}
		 )";

		const GLchar* defaultFragmentShader =
		 R"(#version 450
			in vec2 TexCoord;
			out vec4 frag_color;

			uniform sampler2D texture1;
			uniform sampler2D texture2;

			void main() {
				frag_color = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);
			}
		 )";
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

	void ShaderProgram::CreateShaderProgram(const GLchar* vertexShader, const GLchar* fragmentShader) {
		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);


		glShaderSource(vs, 1, &vertexShader, nullptr);
		glCompileShader(vs);
		this->CheckShaderCompileError(vs);

		glShaderSource(fs, 1, &fragmentShader, nullptr);
		glCompileShader(fs);
		this->CheckShaderCompileError(fs);

		this->Program = glCreateProgram();
		glAttachShader(this->Program, vs);
		glAttachShader(this->Program, fs);
		glLinkProgram(this->Program);

		glDeleteShader(vs);
		glDeleteShader(fs);
	}

	ShaderProgram::ShaderProgram() {
		CreateShaderProgram(this->defaultVertexShader, this->defaultFragmentShader);
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

		CreateShaderProgram(vShaderCode, fShaderCode);
	}

	void ShaderProgram::SetMat4(glm::mat4 mat, const char* name) {
		glUniformMatrix4fv(glGetUniformLocation(this->Program, name), 1, GL_FALSE, glm::value_ptr(mat));
	}

	void ShaderProgram::Set1i(int newInt, const char* name) {
		glUniform1i(glGetUniformLocation(this->Program, name), newInt);
	}


	void ShaderProgram::Use() {
		glUseProgram(this->Program);
	}

}