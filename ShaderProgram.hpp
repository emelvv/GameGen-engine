#pragma once

// https://habr.com/ru/articles/313380/

#include <GLFW/glfw3.h>

namespace Renderer {
	class ShaderProgram {
	public:
		GLuint Program;

		ShaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath);

		void Use();
	private:
		void CheckShaderCompileError(GLuint shader);
	};
}
