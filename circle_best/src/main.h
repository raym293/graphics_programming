#include <array>
#include <cmath>
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unistd.h>
#include <stdlib.h>
using std::vector, std::cout, std::cerr, std::array;
typedef array<GLfloat, 2> vec2f;

const int WIDTH = 800;
const int HEIGHT = 800;
const GLfloat INC = 0.05;
const GLfloat N_TRI = 20; // number of tris per circle
const GLfloat DELTA = 0.0001;
const GLfloat RADIUS = 0.1;
const vec2f GRAVITY = { 0.0, -9.8 };
const int n_balls = 6; // make even for simplicity

void setup1(GLFWwindow* &window);
void setup2(uint& VAO, uint& VBO, vector<GLfloat> vertices, uint& shaderProgram);