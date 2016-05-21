//
//  main.cpp
//  CSCE441-HW4
//
//  Created by Noor Thabit on 3/19/15.
//  Copyright (c) 2015 Noor Thabit. All rights reserved.
//

#include <gl\glut.h>
#include "Insect.h"
#include <math.h>

#define X 0.0f
#define Y 0.0f
#define PI 3.1415926535898
#define Cos(a) cos(PI/180*(a))
#define Sin(a) sin(PI/180*(a))

bool play = false;
double dim = 3.0; /* dimension of orthogonal box */
int windowWidth = 500;
int windowHeight = 450;
int toggleMode = 0; /* projection mode */
int toggleValues = 1; /* toggle values on and off */

int th = 0;   /* azimuth of view angle */
int ph = 0;   /* elevation of view angle */
int fov = 55; /* field of view for perspective */
int asp = 1;  /* aspect ratio */

static Insect worm(5.0, 10.0, 2.0);



void init(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Clear the background of our window to white
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glDepthMask(GL_TRUE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


}




void display(void)
{
	glEnable(GL_BLEND); // Enable the OpenGL Blending functionality
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set the blend mode to blend our current RGBA with what is already in the buffer

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, (GLfloat)windowWidth / (GLfloat)windowHeight, 1.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(X, Y, -21.0);

	if (toggleMode) {
		double Ex = -2 * dim* Sin(th)*Cos(ph);
		double Ey = +2 * dim*Sin(ph);
		double Ez = +2 * dim*Cos(th)*Cos(ph);
		/* camera/eye position, aim of camera lens, up-vector */
		gluLookAt(Ex, Ey, Ez, 0, 0, 0, 0, Cos(ph), 0);
	}
	/*  Orthogonal - set world orientation */
	else {
		glRotatef(ph, 1, 0, 0);
		glRotatef(th, 0, 1, 0);
	}
	worm.display();
	glutSwapBuffers();
}

void project()
{
	glLoadIdentity();

	if (toggleMode) {
		/* perspective */
		gluPerspective(fov, asp, dim / 4, 4 * dim);
	}
	else {
		/* orthogonal projection*/
		glOrtho(-dim*asp, +dim*asp, -dim, +dim, -dim, +dim);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void reshape(int w, int h)
{

	asp = (h > 0) ? (double)w / h : 1;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glLoadIdentity();

	if (w <= h)
		glOrtho(-1.5, 1.5, -1.5*(GLfloat)h / (GLfloat)w,
		1.5*(GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-1.5*(GLfloat)w / (GLfloat)h,
		1.5*(GLfloat)w / (GLfloat)h, -1.5, 1.5, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	project();

}




void keyboard(unsigned char key, int x, int y)
{

	switch (key) {

	case 'm':
		toggleMode = 1 - toggleMode;
		glutPostRedisplay();
		break;

	case '=':
		if (key > 1) {
			fov--;
		}
		glutPostRedisplay();
		break;

	case '-':
		if (key < 179) {
			fov++;
		}
		glutPostRedisplay();
		break;
		//change dimensions
	case 'z':
		dim -= 0.1;
		glutPostRedisplay();
		break;


	case 'Z':
		//elbow = (elbow - 5) % 360;
		dim += 0.1;
		glutPostRedisplay();
		break;



	case 'w':
		worm.moveAllLegsFwdCont();

		glutPostRedisplay();
		break;

	case 's':
		worm.moveAllLegsBackCont();

		glutPostRedisplay();
		break;

	case 'a':
		worm.insectHead.turnHeadleft();

		glutPostRedisplay();
		break;

	case 'd':
		worm.insectHead.turnHeadRight();

		glutPostRedisplay();
		break;
	default:
		break;
	}

}


void windowSpecial(int key, int x, int y){

	/*  Right arrow key - increase azimuth by 5 degrees */
	if (key == GLUT_KEY_RIGHT) th += 5;
	/*  Left arrow key - decrease azimuth by 5 degrees */
	else if (key == GLUT_KEY_LEFT) th -= 5;
	/*  Up arrow key - increase elevation by 5 degrees */
	else if (key == GLUT_KEY_UP) ph += 5;
	/*  Down arrow key - decrease elevation by 5 degrees */
	else if (key == GLUT_KEY_DOWN) ph -= 5;

	/*  Keep angles to +/-360 degrees */
	th %= 360;
	ph %= 360;

	project();
	glutPostRedisplay();

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(50, 50);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(windowSpecial);
	glutMainLoop();
	return 0;
}