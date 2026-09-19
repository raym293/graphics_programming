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

    char* title = "My first window";
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

    while(!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        // render
        glClearColor(f1, f2, f3, 1.0);
        nextColor(f1, f2, f3);
        // cout << f1 << ' ' << f2 <<' ' << f3 << '\n';
        // sleep(1);
        glClear(GL_COLOR_BUFFER_BIT);

        // swp buffers and then poll
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}