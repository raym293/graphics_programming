#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

const GLfloat INC = 0.05;
const float ROATION_INC = 1.0 / M_PI;

void rotate(float *x, float* y) {
    float cos_theta = cos(ROATION_INC);
    float sin_theta = sin(ROATION_INC);

    float x_new = cos_theta*(*x) + -sin_theta*(*y);
    float y_new = sin_theta*(*x) + cos_theta*(*y);

    // fprintf(stderr, "COS: %f\n", cos_theta);
    // fprintf(stderr, "SIN: %f\n", sin_theta);
    
    *x = x_new;
    *y = y_new;
}

void rotateTri(float vertices[]) {
    rotate(&vertices[0], &vertices[1]);
    rotate(&vertices[3], &vertices[4]);
    rotate(&vertices[6], &vertices[7]);
}

void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

GLint drawTriangle(float vertices[], int sz, uint shaderProgram, uint VAO, uint VBO) {
    glBufferData(GL_ARRAY_BUFFER, sz, vertices, GL_STATIC_DRAW);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    return 0; // success is draw
}

const int WIDTH = 800;
const int HEIGHT = 600;
int main() {
    struct timespec ts = {.tv_nsec = 100000000, .tv_sec = 0};
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
    
    // int n_attr;
    // glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n_attr);
    // cout << "HERE: " << n_attr << '\n';
    // cout.flush();
    
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    GLfloat f1 = 0.0, f2 = 0.0, f3 = 0.0;
    
    // vbo -> buffergen -> bind => data
    uint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    uint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    GLfloat vertices[] = {
        0.0,   0.5, 0.0,
        -0.5, -0.5, 0.0,
        0.5, -0.5, 0.0
    };
    
    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "out vec4 vertexColor;\n"
    "void main()\n"
    "{\n"
    "vertexColor = vec4(0.2, 0.5, 0.0, 1.0);\n"
    "gl_Position = vec4(aPos, 1.0);\n"
    "}\0";
    
    const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec4 vertexColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vertexColor;\n"
    "}\0";
    
    uint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    uint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // link 2 shaders
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // debugging these scripts have some weird logging functino glGetProgramiv(...)
    
    // clean up
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    
    // int offsetLocation = glGetUniformLocation(shaderProgram, "offset");
    // glUniform1f(offsetLocation, 0.5);
    
    int ct = 0;
    
    while(!glfwWindowShouldClose(window)) {
        // input
        processInput(window);
        
        // render
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        for(int i = 0; i < 100; i++) {
            drawTriangle(vertices, sizeof(vertices), shaderProgram, VAO, VBO);
            rotateTri(vertices);
        }
        
        // fprintf(stderr, "{%f, %f}; {%f, %f}; {%f, %f}\n", 
        //     vertices[0], vertices[1], vertices[3], vertices[4], vertices[6], vertices[7] );
        
        // if(ct++ > 10) break;
        // swp buffers and then poll
        nanosleep(&ts, NULL);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}