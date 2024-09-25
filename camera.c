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
float lightPosition[] = {1, 1, 3, 1};

float xAlpha = 90;
float zAlpha = 90;

int A=0, D=0, W=0, S=0, Up=0, Down=0, Left=0, Right=0;

int ADD_FRAMETIME = 0;

void playerInput(GLFWwindow* window, int key, int scancode, int action, int mods){

    float verSpeed = 0;
    float horSpeed = 0;

    int stateLeft = glfwGetKey(window, GLFW_KEY_LEFT);
    if (stateLeft == GLFW_PRESS){
        Left = 1;
    }
    else
        Left = 0;
    int stateRight = glfwGetKey(window, GLFW_KEY_RIGHT);
    if(stateRight == GLFW_PRESS){
        Right = 1;
    }
    else
        Right = 0;
    int stateUp = glfwGetKey(window, GLFW_KEY_UP );
    if(stateUp == GLFW_PRESS){
        Up = 1;
    }
    else
        Up = 0;
    int stateDown = glfwGetKey(window, GLFW_KEY_DOWN);
    if(stateDown == GLFW_PRESS){
        Down = 1;
    }
    else
        Down = 0;

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

void moveCamera(){
    float verSpeed = 0;
    float horSpeed = 0;
    if (Left == 1){
        zAlpha++;
    }
    if(Right == 1){
        zAlpha--;
    }
    if(Down == 1){
        xAlpha = ++xAlpha > 180 ? 180 : xAlpha;
    }
    if(Up == 1){
        xAlpha = --xAlpha < 0 ? 0 : xAlpha;
    }

    float angle = -zAlpha / 180 * M_PI;

    if (W == 1){
        verSpeed = 0.1;
    }
    if(S == 1){
        verSpeed = -0.1;
    }
    if(D == 1){
        horSpeed = 0.1;
    }
    if(A == 1){
        horSpeed = -0.1;
    }
    if (verSpeed != 0)
    {
        cameraPosX += sin(angle) * verSpeed;
        cameraPosY += cos(angle) * verSpeed;
    };
    if (horSpeed != 0)
    {
        cameraPosX += sin(angle + 0.5 * M_PI) * horSpeed;
        cameraPosY += cos(angle + 0.5 * M_PI) * horSpeed;
    };
    glRotatef(-xAlpha, 1, 0, 0);
    glRotatef(-zAlpha, 0, 0, 1);

    glTranslatef(-cameraPosX, -cameraPosY, 0);

    printf("%d%d%d%d  %d%d%d%d\n", A, D, W, S, Left, Right, Up, Down);
}
