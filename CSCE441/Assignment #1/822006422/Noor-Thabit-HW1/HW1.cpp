// HW1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <gl/glut.h>

void init(void){
	glClearColor(0.0, 0.0, 0.0, 0.0); // set display windows to white

	glMatrixMode(GL_PROJECTION); // set projection parameters.
	gluOrtho2D(0.0, 600, 0.0, 400);
}

void lineSegment(void){
	glClear(GL_COLOR_BUFFER_BIT); // Clear display windoww.

	glBegin(GL_LINES);

	glColor3f(1, 1, 1); // white
	glVertex2i(250, 235);
	glVertex2i(250, 165); //left vertical

	glColor3f(0, 0.501, 0); //green
	glVertex2i(350, 235);//right vertical
	glVertex2i(350, 165);

	glColor3f(0.501, 0, 0.501); //purple 
	glVertex2i(250, 165);
	glVertex2i(300, 115);//down rigth

	glColor3f(1, 0.647, 0); //orange 
	glVertex2i(350, 165);
	glVertex2i(300, 115);//down left

	glColor3f(1, 1, 0); //yellow
	glVertex2i(250, 235);
	glVertex2i(300, 285);// top left

	glColor3f(35.445, 0.211, 0.149); // brown

	glVertex2i(350, 235);
	glVertex2i(300, 285);// top right
	glEnd();
	glFlush(); // process all OpenGL routines as quickly as possible.

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv); // initialize GLUT.
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //set display mode.
	glutInitWindowPosition(500, 150); // set top-left display windows position.
	glutInitWindowSize(600, 300); // set windows width and height.
	glutCreateWindow("Noor Thabit - Assignment 1"); // Creat Dosplay Window.

	init(); // excute initiallization procedure.
	glutDisplayFunc(lineSegment); // send graphics to display window.
	glutMainLoop(); ///Display everything and wait.


	return 0;
}

