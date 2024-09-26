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

void initLight(){
    glEnable(GL_LIGHTING); //общее освещения для всего пространства
    glShadeModel(GL_SMOOTH);
    float light_ambient[] = { 0.1f, 0.1f, 0.1f, 0.0f };
    float light_diffuse[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    float light_specular[] = { 1.2f, 1.2f, 1.2f, 1.0f };
    //направленного источника
    //убывания интенсивности
    //задействование настроек для источника LIGHT0
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, light_ambient);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glEnable(GL_LIGHT0); // источник света LIGHT0
    glEnable(GL_COLOR_MATERIAL); //разрешения использования
    //материала
    glShadeModel(GL_SMOOTH); // сглаживает границы
    GLfloat material_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat material_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 32.0f };
    GLfloat material_shininess[] = { 50.0f }; //блеск материала
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shininess);
}

void drawCube(){
    GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f
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
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
    };
    glEnable(GL_DEPTH_TEST);
    glColor4f(1, 0.3, 0.3, 1.0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT,0,normals);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void drawObject(int verts){
    float z = 5.0;
    float radius = 2;
    float vertices[verts * 2 * 3 + 2 * 3];
    unsigned int indices[verts * 4 * 3];

    float angle = 2 * M_PI / verts;

    for (int vert = 0; vert < verts; vert++) 
    {
        vertices[vert*3]     = cos(angle * vert) * radius;
        vertices[vert*3 + 1] = sin(angle * vert) * radius;
        vertices[vert*3 + 2] = 0;

    }

    for (int vert = verts; vert < verts * 2; vert++)
    {
        vertices[vert*3]     = cos(angle * vert) * radius;
        vertices[vert*3 + 1] = sin(angle * vert) * radius;
        vertices[vert*3 + 2] = z;

    }

    vertices[verts * 2 * 3]     = 0;
    vertices[verts * 2 * 3 + 1] = 0;
    vertices[verts * 2 * 3 + 2] = 0;
    vertices[verts * 2 * 3 + 3] = 0;
    vertices[verts * 2 * 3 + 4] = 0;
    vertices[verts * 2 * 3 + 5] = 1;

    for (int side = 0; side < verts; side++)
    {

        indices[6 * side] = side;
        indices[6 * side + 1] = side + 1;
        indices[6 * side + 2] = verts + side;

        indices[6 * side + 3] = side + 1;
        indices[6 * side + 4] = verts + side;
        indices[6 * side + 5] = verts+ side + 1;

        if (side+1 == verts)
        {
            indices[6 * side + 1] = 0;
            indices[6 * side + 5] = 0;
        }
    }

    for (int side = 0; side < verts; side++)
    {
        indices[6 * side + verts * 6] = side;
        indices[6 * side + 1 + verts * 6] = side + 1;
        indices[6 * side + 2 + verts * 6] = verts * 2;

        indices[6 * side + 3 + verts * 6] = side + verts;
        indices[6 * side + 4 + verts * 6] = side + verts + 1;
        indices[6 * side + 5 + verts * 6] = verts * 2 + 1;

        if (side+1 == verts)
        {
            indices[6 * side + 1 + verts * 6] = 0;
            indices[6 * side + 4 + verts * 6] = verts;
        }
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glNormalPointer(GL_FLOAT, 0, vertices);
    glDrawElements(GL_TRIANGLES, verts * 3 * sizeof(unsigned int) , GL_UNSIGNED_INT, indices);

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
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
        glVertex3d(0.0f, 20.0f, 0.0f);        //y

        glColor3f(0.0f, 0.0f, 1.0f); //z
        glVertex3d(0.0f, 0.0f, 0.0f);         //z
        glVertex3d(0.0f, 0.0f, 20.0f);        //Z
        glEnd();

    glPopMatrix();
}

void drawChessBoard(int size)
{
    float verteces[] =
    {
         2,  2, 0,
         2, -2, 0,
        -2, -2, 0,
        -2,  2, 0
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    if(size%2 == 0)
        glTranslatef(2, 2, 0);
    glPushMatrix();
        glVertexPointer(3, GL_FLOAT, 0, verteces);
        for (int i = -size / 2; i < size / 2; i++)
        {
            for (int j = -size / 2; j < size / 2; j++)
            {
                glPushMatrix();

                    if ((-1*(i+j)) % 2 == 0) glColor4f (0, 0, 0, 1.0);
                    else glColor4f(1,1,1, 1.0);
                    glTranslatef(i*4, j*4, 0);
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
    float angle = 0.0f;

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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    initLight();
    float lightPosition[] = {1, 1, 3, 1};
    glTranslatef(0, 0, -10);

    while (!glfwWindowShouldClose(window))
    {
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetKeyCallback(window, playerInput);
        processInputEsc(window);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
        moveCamera();
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        drawAxisXYZ();
        drawChessBoard(6);
        drawCube();

        glPushMatrix();
        glColor4f(1.0, 0.2, 0.3, 0.5);
        glTranslatef(2.0, 2.0, 0);
        drawObject(8);
        glPopMatrix();

        glPushMatrix();
        glColor4f(0.2, 0.5, 0.5, 1.0);
        glTranslatef(-2.0, -2.0, 0);
        drawObject(8);
        glPopMatrix();

        glPopMatrix();
        glPushMatrix(); 
        glPopMatrix();
        lightPosition[0] = 10 * sin(angle * 0.1);
        angle += 1.0f;
        glfwSwapBuffers(window);
        glfwPollEvents();
        //sleep_ms(10);
    }
    glfwTerminate();
    return 0;
}
