#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "ShaderProgram.hpp"

namespace Engine {


	class Camera {
	public:

		float yaw = -90, pitch = 0;
		float fov = 60.f;
		float farPlane = 100.f;
		glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 direction = glm::vec3(0.f, 0.f, -1.f);
		glm::vec3 right = glm::vec3(0.f, 1.f, 0.0f);
		

		Camera(float cameraFov, float cameraFarPlane, int* winSizeX, int* winSizeY, glm::vec3 cameraPos, float defaultYaw = -90.f, float defaultPitch = 0.f);
		

		void Update();
		void SetProg(Engine::ShaderProgram* prog);

		Camera(const Camera&) = delete;
		Camera(Camera&&) = delete;
		Camera& operator=(const Camera&) = delete;
		Camera& operator=(Camera&&) = delete;

	private:
		glm::mat4 view = glm::mat4(1.f);
		ShaderProgram* program = nullptr;
		int* windowSizeX = nullptr;
		int* windowSizeY = nullptr;
	};


	Camera::Camera(float cameraFov, float cameraFarPlane, int* winSizeX, int* winSizeY, glm::vec3 cameraPos, float defaultYaw, float defaultPitch) {
		this->yaw = defaultYaw;
		this->pitch = defaultPitch;
		this->pos = cameraPos;
		this->farPlane = cameraFarPlane;
		this->fov = cameraFov;
		this->windowSizeX = winSizeX;
		this->windowSizeY = winSizeY;
	}
	void Camera::SetProg(Engine::ShaderProgram* prog) {
		this->program = prog;
	}


	void Camera::Update() {
		glm::mat4 projection(1.0f);
		projection = glm::perspective(glm::radians(this->fov), (float)(*this->windowSizeX) / (float)(*this->windowSizeY), 0.1f, farPlane);

		(*this->program).SetMat4(projection, "projection");

		this->direction = glm::normalize(glm::vec3(cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch)),
			sin(glm::radians(this->pitch)),
			sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch))));

		this->right = glm::normalize(glm::cross(this->direction, glm::vec3(0.f, 1.f, 0.f)));
		
		this->view = glm::lookAt(this->pos, this->pos + this->direction, glm::vec3(0.f, 1.f, 0.f));
		(*this->program).SetMat4(this->view, "view");
	}
}