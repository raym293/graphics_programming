#include <array>
#include <cmath>
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

const GLfloat INC = 0.05;
const GLfloat N_TRI = 10; // number of tris per circle

typedef array<GLfloat, 2> vec2f;
void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

const int WIDTH = 800;
const int HEIGHT = 800;

vec2f rotateRad(vec2f point, float angleInRadians) {
    const float c = std::cos(angleInRadians);
    const float s = std::sin(angleInRadians);

    GLfloat rot_matrix[2][2] = {
        { c, -s },
        { s, c }
    };

    vec2f new_point {
        rot_matrix[0][0] * point[0] + rot_matrix[0][1] * point[1],
        rot_matrix[1][0] * point[0] + rot_matrix[1][1] * point[1]
    };

    return new_point;
}

vec2f rotateDeg(vec2f point, float angleInDegrees) {
    float angleInRadians = (M_PI * angleInDegrees) / 180.0f;
    return rotateRad(point, angleInRadians);
}


void putCircle(vector<GLfloat>& v, int n_triangles, GLfloat rad, vec2f center) {
    if(n_triangles < 3) {
        cerr << "BRO PUT MORE TRIANGLES\n";
        exit(1);
    }

    float angleInRadians = 2.0f * M_PI / n_triangles;

    // printf("[ANGLE] %f\n", (angleInRadians * 180.0f) / M_PI);

    vec2f line1;
    vec2f line2{rad, 0.0};

    for (int i = 0; i < n_triangles+1; i++)
    {
        line1 = line2;

        line2 = rotateRad( line1, angleInRadians );

        // center
        v.push_back( center[0] + 0.0 );
        v.push_back( center[1] + 0.0 );
        v.push_back( 0.0 );
        
        // line1
        v.push_back( center[0] + line1[0] );
        v.push_back( center[1] + line1[1] );
        v.push_back( 0.0 );
        
        //line2
        v.push_back( center[0] + line2[0] );
        v.push_back( center[1] + line2[1] );
        v.push_back( 0.0 );
    }
}

void putCircle(vector<GLfloat>& v, int n_triangles, GLfloat rad) {
    putCircle(v, n_triangles, rad, {0, 0});
}

void drawArrayTriangles(vector<GLfloat> v) {
    int n_triangles = v.size() / 3;
    for(int i = 0; i < n_triangles; i++) {
        glDrawArrays(GL_TRIANGLES, i*3, 3);
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    const char* title = "My first triangle\n";

    
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

    int framebufferWidth;
    int framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);;

    // vbo -> buffergen -> bind => data
    vector<GLfloat> vertices;

    putCircle(vertices, N_TRI, 0.5);

    // cout << "DEBUG:\n";
    // for(int i = 0; i < vertices.size() / 3; i++) {
    //     printf("POINTS:\n");
    //     printf("{%f, %f, %f}\n", vertices[3*i], vertices[3*i + 1], vertices[3*i + 2]);
    // }
    
    unsigned int shaderProgram;
    uint VAO;
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        uint VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

        const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "gl_Position = vec4(aPos, 1.0);\n"
        "}\0";
        
        const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "FragColor = vec4(1.0, 0.7333333333333333, 0.5411764705882353, 1.0);\n"
        "}\0";

        uint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        uint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        // link 2 shaders
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // debugging these scripts have some weird logging functino glGetProgramiv(...)

        glUseProgram(shaderProgram);

        // clean up
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
    
    while(!glfwWindowShouldClose(window)) {
        // input
        processInput(window);
        
        // render
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        drawArrayTriangles(vertices);

        // swp buffers and then poll
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}