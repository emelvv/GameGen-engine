#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Texture.hpp"
#include "ShaderProgram.hpp"


namespace Engine {
	class Object
	{
	public:
		glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
		float yaw = 0, pitch = 0;

		Object(ShaderProgram* program, float object_points[], const char* textureSrc);
		~Object();


		void SetPosition();
		void SetYaw();
		void SetPitch();
		
		void Draw();


	private:
		ShaderProgram* program;
		Texture* texture1;
		GLuint vao;
		GLuint vbo;
	};

	Object::Object(ShaderProgram* program, float object_points[], const char* textureSrc, glm::vec3 position, float yaw = 0, float pitch = 0)
	{
		this->program = program;

		GLuint vao = 0;
		glGenVertexArrays(1, &vao);
		this->vao = vao;
		glBindVertexArray(vao);


		GLuint vbo = 0;
		glGenBuffers(1, &vbo);
		this->vbo = vbo;
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(object_points), object_points, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * (sizeof(GLfloat)), (GLvoid*)(3 * sizeof(GLfloat)));

		Engine::Texture texture1(textureSrc, GL_TEXTURE_2D, false);
		this->texture1 = &texture1;

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		texture1.unbind();
	}

	Object::~Object()
	{
	}

	void Object::Draw() {
		(*this->program).Use();
		(*this->texture1).bind(0);
		(*program).Set1i(0, "texture1");

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

}