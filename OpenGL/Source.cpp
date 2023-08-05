#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw.h>
#include <IMGUI/imgui_impl_opengl3.h>
#include "ShaderProgram.hpp"
#include "Texture.hpp"


int windowSizeX = 640;
int windowSizeY = 480;


float cube_points[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};


glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
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


static void glfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
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
    glfwSetErrorCallback(glfwErrorCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGL()) {
        std::cout << "Cant load GLAD!" << std::endl;
        return -1;
    }

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;



    bool show_main_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    Renderer::ShaderProgram shader_program("shaders/vShader.glsl", "shaders/fShader.glsl");

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);


    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_points), cube_points, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * (sizeof(GLfloat)), (GLvoid*)(3 * sizeof(GLfloat)));

    //TEXTURE 1
    Renderer::Texture texture1("textures/container.jpg", GL_TEXTURE_2D, false);
    //TEXTURE2
    Renderer::Texture texture2("textures/prapor.jpg", GL_TEXTURE_2D, true);

    shader_program.Use(); // don't forget to activate/use the shader before setting uniforms!
    texture1.bind(0);
    texture2.bind(1);

    glUniform1i(glGetUniformLocation(shader_program.Program, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shader_program.Program, "texture2"), 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //transformation
    glm::mat4 model(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    //projection matrix
    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(60.0f), (float)windowSizeX / (float)windowSizeY, 0.1f, 100.0f);

    //setting camera
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -5.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    //view matrix
    glm::mat4 view;
    view = glm::lookAt(cameraPos, cameraTarget, up);

    //to uniform
    shader_program.SetMat4(model, "model");
    shader_program.SetMat4(view, "view");
    shader_program.SetMat4(projection, "projection");

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark(); // you can also use ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
    ImGui_ImplOpenGL3_Init();

    glEnable(GL_DEPTH_TEST);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Render();


        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);
        for (unsigned int i=0; i < 10; i++) {
            glm::mat4 model(1.f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians((float)glfwGetTime() * 30.0f * (i+1)), glm::vec3(i % 2, 1, i % 2));
            shader_program.SetMat4(model, "model");
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //// render container
        //glBindVertexArray(vao);

        //glm::mat4 model;
        //model = glm::rotate(model, glm::radians((float)glfwGetTime() * 100), glm::vec3(1.f, 0.f, 1.f));
        //shader_program.SetMat4(model, "model");

        //// draw
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(pWindow);
    glfwTerminate();
    return 0;
}