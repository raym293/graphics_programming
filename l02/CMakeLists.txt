cmake_minimum_required(VERSION 3.10)
project(LearnOpenGL)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

# Find Homebrew's GLFW
find_package(glfw3 REQUIRED)

# Compile our main code AND the glad.c loader
add_executable(opengl_app 
    src/main.cpp 
    src/glad.c
)

# Tell the compiler where to find the glad/glad.h headers
target_include_directories(opengl_app PRIVATE include)

# Enable common compiler warnings
target_compile_options(opengl_app PRIVATE -Wall)

# Link GLFW
target_link_libraries(opengl_app PRIVATE glfw)

# Link Mac-specific frameworks required for windowing and graphics
if (APPLE)
    target_link_libraries(opengl_app PRIVATE 
        "-framework OpenGL" 
        "-framework Cocoa" 
        "-framework IOKit" 
        "-framework CoreVideo"
    )
endif()
