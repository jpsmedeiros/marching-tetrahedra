//
//  main.cpp
//
//  Created by Calvin Loncaric on 6/25/11.
//

#include <iostream>
#include <cstdlib>
#include <cassert>

#include "common.h"
#include GLUT_INCLUDE

#include <fstream>
#include <string.h>
using namespace std;

static float rotX;
static float rotY;

static struct { int x, y; } mouse = { -1, -1 };

GLuint list;

void render()
{
    glClearColor(0, 0.3, 0.6, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { 1.0, 1.0, -1.0, 0.0 };
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    //glPushMatrix();
    //glRotatef(rotY, 1, 0, 0);
    //glRotatef(rotX, 0, 1, 0);
    //glutSolidCube(0.15);
    //glPopMatrix();

    glPushMatrix();
    glRotatef(rotY, 1, 0, 0);
    glRotatef(rotX, 0, 1, 0);
    glScalef(0.1, 0.1, 0.1);
    glCallList(list);
    glPopMatrix();

    glutSwapBuffers();
}

void changeSize(int w, int h)
{
    // Prevent a divide by zero, when window is too shor
    // (you cant make a window of zero width).
    if(h == 0)
        h = 1;
    float ratio = 1.0* w / h;

    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective.
    gluPerspective(45,ratio,1,1000);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set up the camera
    gluLookAt(0, 0, -6, 0, 0, 0, 0, 1, 0);
}

void mouseDown(int button, int state, int x, int y)
{
    mouse.x = x;
    mouse.y = y;
}

void mouseDragged(int x, int y)
{

    float dx = x - mouse.x;
    float dy = y - mouse.y;

    rotX += dx / 10;
    rotY -= dy / 10;

    mouse.x = x;
    mouse.y = y;

    glutPostRedisplay();
}

float* extractLineData(std::string line) {
    float *extractedData = new float[4];
    char data[line.size() + 1];
    char * actual;
    strcpy(data, line.c_str());
    actual = strtok(data, ",");
    for (int i = 0; i < 4 && actual != NULL; i++ ){
        actual = strtok (NULL, ",");
        extractedData[i] = atof(actual);
    }
    return extractedData;
}

void setColor(float process) {
    float rgb[8][3] = {
                {1, 0, 0},
                {1, 1, 0},
                {1, 1, 1},
                {0, 1, 1},
                {0, 0, 1},
                {0, 0, 0},
                {0, 1, 0},
                {1, 0, 1} };
    int process_int = static_cast<int>(process);
    glColor3f(rgb[process_int][0], rgb[process_int][1], rgb[process_int][2]);

}

void setSurfaceData() {
    std::ifstream file("/Users/gpergola/files/uff/marching-tetrahedra/projeto-escrita/out.o");
    std::string lineData;
    float * float_points;
    while (std::getline(file, lineData)) {
        float_points = extractLineData(lineData);
        setColor(float_points[3]);
        if (lineData[0] == 's') {
            glVertex3f(float_points[0], float_points[1], float_points[2]);
        }else {
            glNormal3f(float_points[0], float_points[1], float_points[2]);
        }
    }
}

GLuint drawImage() {
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    setSurfaceData();
    glEnd();
    glEndList();
    return list;
}

void init()
{
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    //glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    list = drawImage();
}

void glutInitConfig() {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Marching Tetrahedra");
}

void glutSetFuncs(){
    glutDisplayFunc(render);
    glutReshapeFunc(changeSize);
    glutMouseFunc(mouseDown);
    glutMotionFunc(mouseDragged);
}

void initAndStartLoop(){
    glutInitConfig();
    init();
    glutSetFuncs();
    glutMainLoop();
}

int main (int argc, char * argv[])
{
    glutInit(&argc, argv);
    initAndStartLoop();
    return 0;
}
