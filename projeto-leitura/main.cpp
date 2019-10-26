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

#include "Gyroid.h"
#include "Sphere.h"
#include "Decimate.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
using namespace std;


GLuint preDecimate(const Isosurface& surface,
                   float xMin, float xMax,
                   float yMin, float yMax,
                   float zMin, float zMax,
                   float isolevel,
                   size_t resolution)
{
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);
    decimate(surface, xMin, xMax, yMin, yMax, zMin, zMax, isolevel, resolution);
    glEndList();
    return list;
}

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

    glPushMatrix();
    glRotatef(rotY, 1, 0, 0);
    glRotatef(rotX, 0, 1, 0);
    glutSolidCube(0.15);
    glPopMatrix();

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

void init()
{
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    //Sphere surface
    Gyroid surface;
    int x = 10;
    list = preDecimate(surface, -x, x, -x, x, -x, x, -1, 100);
    cout << list;
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

float* extractLineData(std::string line) {
    float *extractedData = new float[3];
    char data[line.size() + 1];
    char * actual;
    strcpy(data, line.c_str());
    actual = strtok(data, ",");
    for (int i = 0; i < 3 && actual != NULL; i++ ){
        actual = strtok (NULL, ",");
        extractedData[i] = atof(actual);
    }
    return extractedData;
}

void setSurfaceData() {
    std::ifstream file("output.o");
    std::string lineData;
    float * normal;
    float * position;
    while (std::getline(file, lineData)) {
        cout << "Normal Data\n" + lineData + "\n";
        //normal = extractLineData(lineData);
        //glNormal3f(normal[0], normal[1], normal[2]);
        //printf("%f\n%f\n%f\n", normal[0], normal[1], normal[2]);
        std::getline(file, lineData);
        //position = extractLineData(lineData);
        //glVertex3f(position[0], position[1], position[2]);
        //printf("%f\n%f\n%f\n", position[0], position[1], position[2]);
        cout << "Position Data\n" + lineData+"\n";
        //break;
    }
}

void drawImage() {
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);
    setSurfaceData();
    glEndList();
}

int main (int argc, char * argv[])
{
    assert(sizeof(char) == 1);
    glutInit(&argc, argv);
    cout << "Hello, World!\n";
    drawImage();
    initAndStartLoop();
    return 0;
}
