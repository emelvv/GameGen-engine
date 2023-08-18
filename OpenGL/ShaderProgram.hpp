#pragma once
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
		void Set1i(const char* name, int newInt);
		void Set3f(const char* name, float f1, float f2, float f3);
		void Set1f(const char* name, float f1);
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
			layout(location = 2) in vec3 normal;
			out vec2 TexCoord;
			out vec3 Normal;
			out vec3 FragPos;		
			out mat4 View;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;

			uniform vec3 lightPos;

			void main() {
				View = view;

				gl_Position = projection * view * model * vec4(vertex_position, 1.0);
				TexCoord = tex_position;
				Normal = mat3(transpose(inverse(view * model))) * normal;
				FragPos = vec3(view * model * vec4(vertex_position, 1.0));
			}
		 )";

		const GLchar* defaultFragmentShader =
		 R"(#version 450
			in vec2 TexCoord;
			in vec3 Normal;
			in vec3 FragPos;
			in mat4 View;
			out vec4 frag_color; 

			struct Material {
				vec3 ambient;
				vec3 diffuse;
				vec3 specular;
				float shininess;
			}; 

			struct Light {
				vec3 position;

				vec3 ambient;
				vec3 diffuse;
				vec3 specular;
			};
			
			uniform sampler2D texture1;
			uniform sampler2D texture2;
			uniform float ambientStrength;
			uniform float specularStrength;
			uniform Material material;
			uniform Light light;			

			void main() {
				vec3 lightPos = vec3(View * vec4(light.position, 1.0));

				//diffuse
				vec3 norm = normalize(Normal);
				vec3 lightDir = normalize(lightPos - FragPos);
				float diff = max(dot(norm, lightDir), 0.0);
				vec3 diffuse = (diff * material.diffuse) * light.diffuse; 

				//specular
				vec3 viewDir = normalize(-FragPos);
				vec3 reflectDir = reflect(-lightDir, norm); 
				float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
				vec3 specular = material.specular * spec * light.specular;  

				//ambient
				vec3 ambient = material.ambient * light.ambient;

				frag_color = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5) * vec4((diffuse+ambient+specular), 1.0);
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
		this->Use();
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

	void ShaderProgram::Set1i(const char* name, int newInt) {
		glUniform1i(glGetUniformLocation(this->Program, name), newInt);
	}
	void ShaderProgram::Set3f(const char* name, float f1, float f2, float f3) {
		glUniform3f(glGetUniformLocation(this->Program, name), f1, f2, f3);
	}
	void ShaderProgram::Set1f(const char* name, float f1) {
		glUniform1f(glGetUniformLocation(this->Program, name), f1);
	}


	void ShaderProgram::Use() {
		glUseProgram(this->Program);
	}

}