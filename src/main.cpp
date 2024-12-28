#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw.h>
#include <IMGUI/imgui_impl_opengl3.h>
#include "ShaderProgram.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Object.hpp"
#include "ObjectList.hpp"
#include "FPSManager.hpp"


GLfloat cube_points[] = {
    //coordinates         //texture     //normal
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   1.0f,  0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   1.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   1.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   0.0f, -1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,   0.0f, -1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0.0f, -1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   0.0f, -1.0f, 0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,   0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f,  1.0f, 0.0f
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

//window
int windowSizeX = 1920;
int windowSizeY = 1080;
bool cursorIsHidden = true;
bool windowIsFocused = true;
double mouseX, mouseY;
float lastX = 400, lastY = 300;

//deltaTime
float lastFrame;
float deltaTime;

//fps
Engine::FPSManager fps;
int fpsLimit = 144;

//light 
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightColor(1.f, 1.f, 1.f);
float ambientStrength = 0.2f;
float specularStrength = 1.f;


//camera
Engine::Camera camera(70.f, 100.f, &windowSizeX, &windowSizeY, glm::vec3(0.f, 0.f, 6.f));
float cameraSpeed = 6.f;
float cameraSens = 0.065f;
float isLightCubeAttach = true;



void glfwWindowSizeCallback(GLFWwindow* pWindow, int sizeX, int sizeY) {
    windowSizeX = sizeX;
    windowSizeY = sizeY;
    glViewport(0, 0, sizeX, sizeY);
}

static void glfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void keysUpdate(GLFWwindow* window);
void mouseUpdate(GLFWwindow* window);
void windowFocusCallback(GLFWwindow* window, int focused);
void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

int main(void)
{
    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "glfwInit() - failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
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
    glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowFocusCallback(pWindow, windowFocusCallback);
    glfwSetMouseButtonCallback(pWindow, mouseButtonCallback);


    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGL()) {
        std::cout << "Cant load GLAD!" << std::endl;
        return -1;
    }

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;

    //objects
    Engine::ShaderProgram program;

    Engine::ObjectList objs;
    for (int i = 0; i < 10; i++) {
        objs.Add(Engine::Object(&program, cube_points, sizeof(cube_points), "../assets/textures/orange.jpg", cubePositions[i]));
    }

    Engine::Object lightCube(&program, cube_points, sizeof(cube_points), "../assets/textures/white.jpg", glm::vec3(1.2f, 1.0f, 2.0f));
    lightCube.scale = glm::vec3(0.2f);

    Engine::Object platform(&program, cube_points, sizeof(cube_points), "../assets/textures/gray.jpg", glm::vec3(0.f, -4.f, 0.f));
    platform.scale = glm::vec3(100.f, 1.f, 100.f);


    //setup camera
    camera.SetProg(&program);

    //light
    program.Set3f("lightColor", lightColor.x, lightColor.y, lightColor.z);


    // Setup ImGui context
    bool show_main_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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
        //calculating deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        //input (�������� ����� ��� glfwKeyCallback)
        keysUpdate(pWindow);
        mouseUpdate(pWindow);

        /* Render here */
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // creating cubes
        for (int i = 0; i < objs.Len(); i++) {
            objs[i].angle = glm::radians((float)glfwGetTime() * 30.0f * (i + 1)*100);
            objs[i].rotationDirection = glm::vec3(i % 2, 1, i % 2);
        }

        objs.DrawAll();
        platform.Draw();


        //light
        if (isLightCubeAttach) {
            lightPos = camera.pos + camera.direction * 6.f;
            lightCube.position = lightPos;
        }
        program.Set3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
        program.Set1f("ambientStrength", ambientStrength);
        program.Set1f("specularStrength", specularStrength);
        program.Set3f("viewPos", camera.pos.x, camera.pos.y, camera.pos.z);
        lightCube.Draw();
        
        //setup imgui

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowSize(ImVec2(500, 200));
        ImGui::Begin("Debug Menu");
        ImGui::Text("FPS: %d Yaw: %f Pitch: %f", fps.FPS, camera.yaw, camera.pitch);
        ImGui::Text("pos.x: %f pos.y: %f pos.z: %f", camera.pos.x, camera.pos.y, camera.pos.z);
        ImGui::SliderInt("Fps Limit", &fpsLimit, 10.0f, 200.0f);
        ImGui::SliderFloat("Movement Speed", &cameraSpeed, 0.0f, 10.0f);
        ImGui::SliderFloat("Mouse Sensitivity", &cameraSens, 0.0f, 1.0f);
        ImGui::SliderFloat("FOV", &camera.fov, 50.f, 120.f);
        ImGui::SliderFloat("Far Plane", &camera.farPlane, 60.f, 200.f);
        ImGui::SliderFloat("Ambient Strength", &ambientStrength, 0.f, 1.f);
        ImGui::SliderFloat("Specular Strength", &specularStrength, 0.f, 3.f);
        ImGui::End();

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ////view matrix
        camera.Update();

        //fps
        fps.Calc();
        fps.Limit(fpsLimit);

        //end
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

void keysUpdate(GLFWwindow* window)
{
    float speed = cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        speed = speed * 2;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.pos += speed * camera.direction * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.pos -= speed * camera.direction * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.pos -= camera.right * speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.pos += camera.right * speed * deltaTime;
}

void mouseUpdate(GLFWwindow* window) {
    glfwGetCursorPos(window, &mouseX, &mouseY);
    if (!windowIsFocused || !cursorIsHidden)
        return;
    float xoffset = (float)mouseX - lastX;
    float yoffset = lastY - (float)mouseY;
    lastX = mouseX;
    lastY = mouseY;

    camera.yaw += xoffset * cameraSens;
    camera.pitch += yoffset * cameraSens;

    if (camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if (camera.pitch < -89.0f)
        camera.pitch = -89.0f;
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS && cursorIsHidden) {
        cursorIsHidden = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    }
    else if (key == GLFW_KEY_F && action == GLFW_PRESS && !cursorIsHidden) {
        cursorIsHidden = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

}


void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && isLightCubeAttach) {
        isLightCubeAttach = false;
    }
    else {
        isLightCubeAttach = true;
    }

}


void windowFocusCallback(GLFWwindow* window, int focused)
{
    if (focused)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        cursorIsHidden = true;
        windowIsFocused = true;
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        cursorIsHidden = false;
        windowIsFocused = false;
    }
}