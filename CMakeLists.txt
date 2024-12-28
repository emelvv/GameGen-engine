cmake_minimum_required(VERSION 3.10)
project(TEST)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


include_directories(include)

# glfw
add_subdirectory(external/glfw-3.4)

# glad
include_directories("external/glad/include")
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

#glm
include_directories(external/glm/include)

#imgui
set(IMGUI_SOURCES
    external/imgui/include/imgui/imgui.cpp
    external/imgui/include/imgui/imgui_draw.cpp
    external/imgui/include/imgui/imgui_widgets.cpp
    external/imgui/include/imgui/imgui_tables.cpp
    external/imgui/include/imgui/imgui_impl_glfw.cpp
    external/imgui/include/imgui/imgui_impl_opengl3.cpp
)
include_directories(external/imgui/include)

#stb
include_directories(external/stb/include)

add_executable(app "src/main.cpp" ${IMGUI_SOURCES})
find_package(OpenGL REQUIRED)


target_link_libraries(app PRIVATE glfw OpenGL::GL glad)


