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
		float yaw, pitch;

		Object(ShaderProgram* program, float object_points[], const char* textureSrc, glm::vec3 position, float yaw = 0.f, float pitch = 0.f);
		~Object();
		
		void Draw();


	private:
		ShaderProgram* program;
		Texture* texture1;
		GLuint vao;
		GLuint vbo;
	};

	Object::Object(ShaderProgram* program, float object_points[], const char* textureSrc, glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), float yaw, float pitch)
	{
		this->program = program;
		this->position = position;
		this->yaw = yaw;
		this->pitch = pitch;

		this->vao = 0;
		glGenVertexArrays(1, &this->vao);
		glBindVertexArray(this->vao);


		this->vbo = 0;
		glGenBuffers(1, &this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
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
		(*this->program).Set1i(0, "texture1");

		glBindVertexArray(this->vao);

		//model matrix
		glm::mat4 model(1.f);
		model = glm::translate(model, this->position);
		model = glm::rotate(model, (float)glfwGetTime() * 30.0f * 4, glm::vec3(1.0f, 1.0f, 0.0f));
		(*this->program).SetMat4(model, "model");


		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

}