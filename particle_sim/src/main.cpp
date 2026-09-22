#include "main.h"

class ball {
public:
    vec2f center;
    GLfloat radius;
    vec2f velocity;
    ball() {  };
    ball(vec2f center, GLfloat radius): center(center), radius(radius), velocity({0.0,0.0}) {   };
    
    float getRandomVelocityChange() {
        // uses TEMPERATURE
        float r = (random() % INT_MAX) / (float)(INT_MAX);
        r *= TEMPERATURE;
        r *= (random() % 2 == 0 ? 1 : -1);
        return r;
    }

    void update_pos() {
        // printf("BALL CENTER: {%f, %f}\n", center[0], center[1]);
        center[0] += velocity[0];
        center[1] += velocity[1];

    }
    
    void update_vel() {

        
        if( abs(center[0]) > 1 - radius /*&& cur.tv_usec - t.tv_usec > 1000*/) {
            velocity[0] = -velocity[0];
        }
        if( abs(center[1]) > 1 - radius /*&& cur.tv_usec - t.tv_usec > 1000*/) {
            velocity[1] = -velocity[1];
        }
        
        if(center[1] < -1+RADIUS) {
            center[1] = -1+RADIUS+DELTA;
        }
        
        
        velocity[0] += GRAVITY[0] * DELTA + getRandomVelocityChange();
        velocity[1] += GRAVITY[1] * DELTA + getRandomVelocityChange();
        
        velocity[0] *= DAMP;
        velocity[1] *= DAMP;
    }
    
    void collision(ball& otherBall) {
        if(dist(otherBall.center, center) < 2*radius) {
        }
    }
};

void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
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
    
    vec2f line1;
    vec2f line2{rad, 0.0};
    
    for (int i = 0; i < n_triangles; i++)
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
    
int main() {
    srandom(SEED);
    GLFWwindow* window;
    setup1(window);
    vector<GLfloat> vertices;
    
    
    unsigned int shaderProgram;
    uint VAO;
    uint VBO;
    
    setup2(VAO, VBO, vertices, shaderProgram);
    
    vector<vector<ball>> balls(N_ROWS, vector<ball>(N_COLS, ball()));
    
    for(int i = 0; i < N_ROWS; i++) {
        for (int j = 0; j < N_COLS; j++)
        {
            GLfloat left_offset = RADIUS - N_COLS * (RADIUS + PADDING);
            balls[i][j] = ball( vec2f{ left_offset + 2*j*RADIUS + 2*j*PADDING, -0.5f + RADIUS*2*i + 2*i*PADDING}, RADIUS );
        }
        
    }
    
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    while(!glfwWindowShouldClose(window)) {
        // input
        processInput(window);
        
        // render
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        vertices.clear();
        
        for ( int i = 0; i < N_ROWS; i++ )
        {
            for( int j = 0; j < N_COLS; j++) {
                putCircle(vertices, N_TRI, balls[i][j].radius, balls[i][j].center);
                balls[i][j].update_pos();
                balls[i][j].update_vel();

            }
            
        }
        
        
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        timespec ts{.tv_nsec = 10000000, .tv_sec = NULL};
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

    // glUseProgram(shaderProgram);

    // clean up
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}