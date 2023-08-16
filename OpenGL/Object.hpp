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
		float angle = 0.f;
		float scale = 1.f;
		glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 rotationDirection = glm::vec3(0.f, 0.f, 0.f);

		Object(ShaderProgram* program, float objectPoints[], int pointsSize, const char* textureSrc, glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), float angle = 0.f, glm::vec3 rotationDirection = glm::vec3(0.f, 1.f, 0.f));
		~Object();
		
		void Draw();


	private:
		ShaderProgram* program;
		Texture* texture;
		GLuint vao;
		GLuint vbo;
	};

	Object::Object(ShaderProgram* program, float objectPoints[], int pointsSize, const char* textureSrc, glm::vec3 position, float angle, glm::vec3 rotationDirection)
	{
		this->program = program;
		this->position = position;
		this->rotationDirection = rotationDirection;
		this->angle = angle;

		this->vao = 0;
		glGenVertexArrays(1, &this->vao);
		glBindVertexArray(this->vao);


		this->vbo = 0;
		glGenBuffers(1, &this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, pointsSize, objectPoints, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), nullptr);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * (sizeof(GLfloat)), (GLvoid*)(3 * sizeof(GLfloat)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * (sizeof(GLfloat)), (GLvoid*)(5 * sizeof(GLfloat)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		this->texture = new Engine::Texture(textureSrc, GL_TEXTURE_2D, false);
	}

	
	Object::~Object()
	{
	}

	void Object::Draw() {
		(*this->program).Use();

		//1
		(*this->texture).bind(0);
		(*this->program).Set1i("texture1", 0);

		glBindVertexArray(this->vao);

		//model matrix
		glm::mat4 model(1.f);
		model = glm::translate(model, this->position);
		model = glm::rotate(model, glm::radians(angle), rotationDirection);
		model = glm::scale(model, glm::vec3(scale));
		(*this->program).SetMat4(model, "model");

		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		(*this->texture).unbind();

	}

}