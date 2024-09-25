#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
#else
#include <unistd.h> // for usleep
#include <time.h>
#endif

void sleep_ms(int milliseconds){ // cross-platform sleep function
#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    if (milliseconds >= 1000)
      sleep(milliseconds / 1000);
    usleep((milliseconds % 1000) * 1000);
#endif
}


#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include <time.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <GL/glx.h>    
#include <stdlib.h>
#include <stdio.h>
#include "camera.c"
#define true 1
#define false 0
#define GLEW_STATIC
#define WINDOW_WIDTH 1600 
#define WINDOW_HEIGHT 900 

void drawCube(){
    GLfloat vertices[] = {
        1.0f, 1.0f, 5.1f,
        -1.0f, 1.0f, 5.1f,
        -1.0f,  -1.0f, 5.1f,
        1.0f,  -1.0f, 5.1f,
        1.0f, 1.0f, 0.1f,
        -1.0f, 1.0f, 0.1f,
        -1.0f,  -1.0f, 0.1f,
        1.0f,  -1.0f, 0.1f,
    };

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0,
        1, 5, 6,
        6, 2, 1,
        7, 6, 5,
        5, 4, 7,
        4, 0, 3,
        3, 7, 4,
        4, 5, 1,
        1, 0, 4,
        3, 2, 6,
        6, 7, 3
    };
    GLfloat normals[] = {
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f,  1.0f,
        -1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
    };
    glColor4f(1, 0.3, 0.3, 1.0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glNormalPointer(GL_FLOAT, 0, normals);
    glDrawElements(GL_TRIANGLES, 9*sizeof(unsigned int), GL_UNSIGNED_INT, indices);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}


void processInputEsc(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

void drawAxisXYZ()
{
    glPushMatrix();

        glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f); //x
        glVertex3d(0.0f, 0.0f, 0.0f);         //x
        glVertex3d(20.0f, 0.0f, 0.0f);        //x

        glColor3f(0.0f, 1.0f, 0.0f); //y
        glVertex3d(0.0f, 0.0f, 0.0f);         //y
        glVertex3d(0.0f, 10.0f, 0.0f);        //y

        glColor3f(0.0f, 0.0f, 1.0f); //z
        glVertex3d(0.0f, 0.0f, 0.0f);         //z
        glVertex3d(0.0f, 0.0f, 10.0f);        //Z
        glEnd();

    glPopMatrix();
}

void drawChessBoard(float size)
{
    float verteces[] =
    {
         1,  1, 0,
         1, -1, 0,
        -1, -1, 0,
        -1,  1, 0
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glPushMatrix();

        if (fmod(size, 2) != 1)
        {
            glTranslatef(1,1,0);
        }

        glVertexPointer(3, GL_FLOAT, 0, verteces);
        for (int i = -size / 2; i < size / 2; i++)
        {
            for (int j = -size / 2; j < size / 2; j++)
            {
                glPushMatrix();

                    if ((i+j) % 2 == 0) glColor4f (0, 0, 0, 0.8);
                    else glColor4f(1,1,1, 0.8);
                    glTranslatef(i*2, j*2, 0);
                    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

                glPopMatrix();

            }
        }

    glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
};

int main(){
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "KMS", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glfwMakeContextCurrent(window);
    float theta = 0.0f;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 1, 80);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    glShadeModel(GL_SMOOTH);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    float light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    float light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float light_specular[] = { 0.2f, 0.2f, 0.2f, 32.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glTranslatef(0, 0, -10);

    while (!glfwWindowShouldClose(window))
    {
        glfwSetKeyCallback(window, playerInput);
        processInputEsc(window);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
        moveCamera();
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        drawAxisXYZ();
        drawChessBoard(20);
        drawCube();
        glPopMatrix();
        glPushMatrix(); 
        glScalef(1, 1, 1);
        glTranslatef(0, 0, 0.5);
        glPopMatrix();

        glfwSwapBuffers(window);
        glfwPollEvents();
            sleep_ms(10);
    }
    glfwTerminate();
    return 0;
}
