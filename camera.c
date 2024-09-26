#include <math.h>
#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <GL/glx.h>
#include <stdio.h>    
#define true 1
#define false 0
#define GLEW_STATIC
#define WINDOW_WIDTH 1600 
#define WINDOW_HEIGHT 900 

float cameraPosX = 0.0, cameraPosY = 0.0; 

double yaw = 60;
double pitch = 30;
double xMove = 0;
double yMove = 0;
double lastX =  1600.0f / 2.0;
double lastY =  900.0 / 2.0;

int firstMouse = 0;

int A=0, D=0, W=0, S=0;

int ADD_FRAMETIME = 0;

void playerInput(GLFWwindow* window, int key, int scancode, int action, int mods){

    int stateW = glfwGetKey(window, GLFW_KEY_W);
    if (stateW == GLFW_PRESS){
        W = 1;
    }
    else
        W = 0;
    int stateS = glfwGetKey(window, GLFW_KEY_S);
    if(stateS == GLFW_PRESS){
        S = 1;
    }
    else
        S = 0;
    int stateD = glfwGetKey(window, GLFW_KEY_D );
    if(stateD == GLFW_PRESS){
        D = 1;
    }
    else
        D = 0;
    int stateA = glfwGetKey(window, GLFW_KEY_A);
    if(stateA == GLFW_PRESS){
        A = 1;
    }
    else
        A = 0;
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos){

    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    double xoffset = xPos - lastX;
    double yoffset = -lastY + yPos; // reversed since y-coordinates go from bottom to top
    lastX = xPos;
    lastY = yPos;

    double sensitivity = 0.05f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    pitch += xoffset;
    yaw += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (yaw > 90.0f)
        yaw = 90.0f;
    if (yaw < 0.0f)
        yaw = 0.0f;
}

void moveCamera(){
    float pitchRadian = -pitch / 180 * M_PI;
    if (W == 1){
        xMove = 0.1;
    }
    if(S == 1){
        xMove = -0.1;
    }
    if(D == 1){
        yMove = 0.1;
    }
    if(A == 1){
        yMove = -0.1;
    }
    if (xMove)
    {
        cameraPosX += sin(pitchRadian) * xMove;
        cameraPosY += cos(pitchRadian) * xMove;
    };
    if (yMove)
    {
        cameraPosX += sin(pitchRadian + 0.5 * M_PI) * yMove;
        cameraPosY += cos(pitchRadian + 0.5 * M_PI) * yMove;
    };
    glRotatef(-yaw, 1, 0, 0);
    glRotatef(-pitch, 0, 0, 1);
    glTranslatef(-cameraPosX, -cameraPosY, 0);
    xMove = 0;
    yMove = 0;
    printf("%d%d%d%d\n", A, D, W, S);
}

