#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <STB/stb_image.h>
#include "ShaderProgram.hpp"


int windowSizeX = 640;
int windowSizeY = 480;


GLfloat points[] = {     // colors        // texture coords
    -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  -1.0f,  1.0f,
     0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   1.0f,  1.0f,
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, -1.0f,
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  -1.0f, -1.0f
};


void glfwWindowSizeCallback(GLFWwindow* pWindow, int sizeX, int sizeY) {
    windowSizeX = sizeX;
    windowSizeY = sizeY;
    glViewport(0, 0, sizeX, sizeY);
}


void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == 1) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}


int main(void)
{
    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "glfwInit() - failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(windowSizeX, windowSizeY, "GameGen", NULL, NULL);
    if (!pWindow)
    {
        std::cout << "glfwCreateWindow - failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGL()) {
        std::cout << "Cant load GLAD!" << std::endl;
        return -1;
    }

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;


    Renderer::ShaderProgram shader_program("shaders/vShader.glsl", "shaders/fShader.glsl");

    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);


    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);


    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), nullptr);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*(sizeof(GLfloat)), (GLvoid*)(3*sizeof(GLfloat)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*(sizeof(GLfloat)), (GLvoid*)(6*sizeof(GLfloat)));


    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind Texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // render container
        shader_program.Use();
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}