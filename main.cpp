#include <GL/glut.h>
#include <mpi.h>
#include <stdio.h>

void drawWindow(){
	glClearColor(1.0f, 0.5f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

int main(int argc, char** argv) {    
	MPI_Init(NULL, NULL);
	glutInit(&argc, argv);
	printf("Inicializando projeto\n");

	MPI_Finalize();

	glutCreateWindow("Olá Mundo OpenGL");
	glutDisplayFunc(drawWindow);
	glutMainLoop();	

	return 0;
}