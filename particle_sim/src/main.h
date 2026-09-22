#include <array>
#include <cmath>
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
using std::vector, std::cout, std::cerr, std::array;
typedef array<GLfloat, 2> vec2f;

const int WIDTH = 800;
const int HEIGHT = 800;
const GLfloat INC = 0.05;
const GLfloat N_TRI = 20; // number of tris per circle
const GLfloat DELTA = 0.0001;
const GLfloat RADIUS = 0.05;
const vec2f GRAVITY = { 0.0, -9.8 };
const GLfloat DAMP = 0.999;
const int n_balls = 20; 

void setup1(GLFWwindow* &window);
void setup2(uint& VAO, uint& VBO, vector<GLfloat> vertices, uint& shaderProgram);
GLfloat dist(vec2f a, vec2f b) {    return sqrt( pow((a[0] - b[0]), 2) + pow((a[1] - b[1]), 2) );   }
// vec2f normal(vec2f a, vec2f b) { return }