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
void setup1(GLFWwindow* &window);
void setup2(uint& VAO, uint& VBO, vector<GLfloat> vertices, uint& shaderProgram);

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

void update_pos(vec2f &center, vec2f vel) {
    center[0] += vel[0];
    center[1] += vel[1];
}

void update_vel(vec2f &center, vec2f& vel, vec2f& accel, GLfloat rad) {
    if( abs(center[0]) > 1 - rad ) {
        vel[0] = -vel[0];
    }
    if( abs(center[1]) > 1 - rad ) {
        vel[1] = -vel[1];
    }
}

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
    // vbo -> buffergen -> bind => data
    GLFWwindow* window;
    setup1(window);
    vector<GLfloat> vertices;

    
    unsigned int shaderProgram;
    uint VAO;
    uint VBO;
    
    const GLfloat rad = 0.1;
    vec2f center = { 0.0, 0.0 };
    vec2f velocity = { 0.2, -0.1 };
    vec2f acceleration = { 0.0, 0.0 };
    putCircle(vertices, N_TRI, rad, center);
    setup2(VAO, VBO, vertices, shaderProgram);
    
    
    
    while(!glfwWindowShouldClose(window)) {
        // input
        processInput(window);
        
        // render
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        vertices.clear();
        update_pos(center, velocity);
        update_vel(center, velocity, acceleration, rad);
        
        putCircle(vertices, N_TRI, rad, center);
        drawArrayTriangles(vertices);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);


        // swp buffers and then poll
        glfwSwapBuffers(window);
        glfwPollEvents();
        timespec ts{.tv_nsec = 50000000, .tv_sec = 0};
        nanosleep(&ts, NULL);
    }

    glfwTerminate();
    return 0;
}

void setup1(GLFWwindow* &window) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    const char* title = "My first triangle\n";

    
    window = glfwCreateWindow(WIDTH, HEIGHT, title, NULL, NULL);

    if(window == NULL) {
        cerr << "ERROR: TO CREATE WINDOW\n";
        return;
    }
    
    glfwMakeContextCurrent(window);
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cerr << "ERROR: FAILED TO LOAD PROC ADRESS\n";
        return;
    }

    int framebufferWidth;
    int framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void setup2(uint& VAO, uint& VBO, vector<GLfloat> vertices, uint& shaderProgram) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

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