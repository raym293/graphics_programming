#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

const GLfloat INC = 0.05;

void nextColor(GLfloat& f1, GLfloat& f2, GLfloat& f3) {
    int rem = (random() % 3LL);

    switch (rem)
    {
    case 0:
        f1 += INC;
        if(f1 > 1.00) f1 = 0;
        break;
    case 1:
        f2 += INC;
        if(f2 > 1.00) f2 = 0;
        break;
    case 2:
        f3 += INC;
        if(f3 > 1.00) f3 = 0;
        break;
    }
}

void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

const int WIDTH = 800;
const int HEIGHT = 600;
int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    const char* title = "My first triangle\n"
    "does this print?";

    
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, title, NULL, NULL);

    if(window == NULL) {
        cerr << "ERROR: TO CREATE WINDOW\n";
        return 1;
    }
    
    glfwMakeContextCurrent(window);
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cerr << "ERROR: FAILED TO LOAD PROC ADRESS\n";
        return 1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    GLfloat f1 = 0.0, f2 = 0.0, f3 = 0.0;

    // vbo -> buffergen -> bind => data
    GLfloat vertices[] = {
        0.25,   0.5, 0.0,
        0.0, -0.5, 0.0,
        0.5, -0.5, 0.0,
        -0.5,   -0.5, 0.0,
        -0.25, 0.5, 0.0,
        0.0, -0.5, 0.0
    };

    uint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    uint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

    const char *fragmentShaderSource1 = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";
    const char *fragmentShaderSource2 = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(0.5f, 0.2f, 0.2f, 1.0f);\n"
    "}\0";

    uint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    uint fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
    glCompileShader(fragmentShader1);

    uint fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader2);

    // link 2 shaders
    unsigned int shaderProgram1;
    shaderProgram1 = glCreateProgram();
    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader1);
    glLinkProgram(shaderProgram1);

    unsigned int shaderProgram2;
    shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);

    // debugging these scripts have some weird logging functino glGetProgramiv(...)

    // clean up
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader1);
    glDeleteShader(fragmentShader2);

    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        // render
        glClearColor(f1, f2, f3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram1);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glUseProgram(shaderProgram2);
        glDrawArrays(GL_TRIANGLES, 3, 3);

        // nextColor(f1, f2, f3);
        // cout << f1 << ' ' << f2 <<' ' << f3 << '\n';
        // sleep(1);

        // swp buffers and then poll
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}