#pragma once
#include <GLFW/glfw3.h>
#include <GLAD/glad.h>
#include <STB/stb_image.h>
#include <iostream>

namespace Engine {

	class Texture
	{
	public:
		GLuint texture = 0;
		int width;
		int height;

		Texture(const char* imageSrc, GLenum textureType, bool rotate);
		~Texture();

		void bind(GLint n);
		void unbind();

	private:
		GLenum textureType;
	};

	Texture::Texture(const char* imageSrc, GLenum type, bool flip)
	{
		this->textureType = type;

		glGenTextures(1, &this->texture);
		glBindTexture(this->textureType, this->texture);

		glTexParameteri(this->textureType, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(this->textureType, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(this->textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(this->textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		stbi_set_flip_vertically_on_load(flip);
		int nrChannels1;
		unsigned char* data1 = stbi_load(imageSrc, &this->width, &this->height, &nrChannels1, 0);

		if (data1)
		{
			glTexImage2D(this->textureType, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
			glGenerateMipmap(this->textureType);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		
		glActiveTexture(0);
		glBindTexture(this->textureType, 0);


		stbi_image_free(data1);
	}

	void Texture::bind(GLint n) {
		glActiveTexture(GL_TEXTURE0 + n);
		glBindTexture(this->textureType, this->texture);
		glActiveTexture(0);
	}

	void Texture::unbind() {
		glActiveTexture(0);
		glBindTexture(this->textureType, 0);
	}


	Texture::~Texture()
	{
		glDeleteTextures(1, &this->texture);
	}
}