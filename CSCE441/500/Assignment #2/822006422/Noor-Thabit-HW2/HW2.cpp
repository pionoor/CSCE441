// HW2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <gl\glut.h>

int rgb = 0;
static GLfloat spin = 0.0;
GLfloat spinSpeed = 0.01;
GLfloat homeY = 0.0;
GLfloat homeX = 0.0;
GLfloat r, g, b = 0.5;
int polyShapeMode = 3; // 1 for drawing the polygon as vertices only, 2 for lines, 3 full polygon
bool middleClick = false; // used in glutMotionFunc to detect if the middle click is pressed.


void init(void)
{
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glShadeModel(GL_FLAT);
}

void drawPoly(){
	glClear(GL_COLOR_BUFFER_BIT);
	switch (polyShapeMode)
	{
	case 1:
		glBegin(GL_POINTS);
		break;
	case 2:
		glBegin(GL_LINE_LOOP);
		break;
	case 3:
		glBegin(GL_POLYGON);
		break;

	default:
		break;
	}
	glVertex2i(-75, 60);	
	glVertex2i(0, 110);
	glVertex2i(80, 25);
	glVertex2i(10, -100);
	glEnd();
	glFlush();
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(spin, 0.0, 0.0, 1.0);
	glColor3f(1.0, 1.0, 1.0);
	drawPoly(); 
	glPopMatrix();
	glutSwapBuffers();
	glEnd();
	glFlush();
}

void spinRightDisplay(void)
{
	spin = spin - spinSpeed;
	if (spin > 360.0)
		spin = spin - 360.0;
	glutPostRedisplay();

}

void spinLeftDisplay(void)
{
	spin = spin + spinSpeed;
	if (spin > 360.0)
		spin = spin - 360.0;
	glutPostRedisplay();

}
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-125, 125, -125, 125, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void MotionClick(int x, int y){ // adujust the background colour and the spining speed

	GLfloat newY, newX = 0;
	newY = homeY - y ;
	newX = x - homeX;
	if (newY != 0 && spinSpeed > 0.0 && !middleClick){ // adjust the rotation speed
		spinSpeed += newY/255;		
	}
	if (newX != 0 && !middleClick){ // adjust the background intensity
		r, g, b += newX/500;
		rgb += newX/ 25500;
		glClearColor(r, g, b, 1.0);

	}

	if (middleClick){
		glTranslatef(newX, newY, 0.0);
		glutPostRedisplay();
	}
	{

	}
	homeX = x;
	homeY = y;
}




void mouse(int button, int state, int x, int y)
{	
	homeX = x; //save the original x, and y before moving the mouse
	homeY = y;
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN){
			glutIdleFunc(spinLeftDisplay);
			MotionClick(x, y);
		}
		else{
			glutIdleFunc(NULL);
			spinSpeed = 0.01;
		}

		break;

	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN){
			glutIdleFunc(spinRightDisplay);
			MotionClick(x, y);
		}
		else{
			glutIdleFunc(NULL);
			spinSpeed = 0.01;
		}
		break;

	case GLUT_MIDDLE_BUTTON: 
		if (state == GLUT_DOWN){
			middleClick = true;
			glutIdleFunc(NULL);
		}
		else{
			middleClick = false;
			}
		break;
	default:
		break;
	}
}

void keyboard(unsigned char key, int x, int y){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (key)
	{
	case '1':
		polyShapeMode = 1;
		break;
	case '2':
		polyShapeMode = 2;
		break;
	case '3':
		polyShapeMode = 3;
		break;
	case 'c':
		r = 0.0;
		g = 0.5;
		b = 0.5;
		glClearColor(r, g, b, 1.0);
		break;
	case 'm':
		r = 0.5;
		g = 0.0;
		b = 0.5;
		glClearColor(r, g, b, 1.0);
		break;
	case 'y':
		r = 0.8;
		g = 0.8;
		b = 0.0;
		glClearColor(r, g, b, 1.0);
		break;
	case 'w':
		r = 1.0;
		g = 1.0;
		b = 1.0;
		glClearColor(r, g, b, 1.0);
		break;
	default:
		break;
	}

	glutPostRedisplay();
	

}

/*
*  Request double buffer display mode.
*  Register mouse input callback functions
*/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(250, 250);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Noor Thabit - Assignment 2");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(MotionClick);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
