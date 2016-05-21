//
//  Body.h
//  CSCE441-HW4
//
//  Created by Noor Thabit on 3/20/15.
//  Copyright (c) 2015 Noor Thabit. All rights reserved.
//


#ifndef CSCE441_HW4_Body_h
#define CSCE441_HW4_Body_h
#include <gl\glut.h>

class Body {
public:
	GLfloat x, y, z; //coordinates
	GLfloat w, h, d; //width, height, depth.
	GLfloat angle = 0;
	GLfloat translateX;

	GLfloat moveX, moveY = 0;

	bool reverse = false;


	void set_XYZ(GLfloat _x, GLfloat _y, GLfloat _z){
		x = _x;
		y = _y;
		z = _z;
	}

	void setWHD(GLfloat _w, GLfloat _h, GLfloat _d){
		w = _w;
		h = _h;
		d = _d;
	}

	void moveTailRight(){

		if (angle <= 25) {
			angle += 2.5;
			if (angle == 25){
				reverse = true;
			}
		}

		displayTail();
	}

	void moveTailLeft(){

		if (angle >= -25) {
			angle -= 2.5;
			if (angle == -25){
				reverse = false;

			}
		}

		displayTail();
	}

	void contMoveTail(){
		if (!reverse) {
			moveTailRight();

		}
		if (reverse) {
			moveTailLeft();

		}
	}


	void translateUp(){
		moveX += 0.1;

	}

	void translateDown(){
		moveX -= 0.1;

	}

	void displaySegments()
	{
		glPushMatrix();
		glTranslatef(moveY, moveX, 0);


		glPushMatrix();
		glColor4f(1.0f, 0.9f, 0.8f, 1.0f);
		glTranslatef(0, -4.0, 0);
		glScalef(2.0*w, 1.5*h, 6 * d);
		glutSolidSphere(0.2, 50, 50);
		glPopMatrix();

		glPushMatrix();
		glColor4f(1.0f, 0.9f, 0.8f, 1.0f);
		glScalef(2.0*w, 1.5*h, 6 * d);
		glutSolidSphere(0.2, 50, 50);
		glPopMatrix();

		glPopMatrix();


	}

	void displayTail(){

		glPushMatrix();
		glTranslatef(0, moveX, 0);
		//tail
		glPushMatrix();
		glTranslatef(translateX, 0, 0);
		glRotated((GLfloat)angle, 0, 1, 0);

		glPushMatrix();
		glColor4f(0.55f, 0.2f, 0.3f, 1.0f);
		glRotatef((GLfloat)70, 1, 0, 0);
		glTranslatef(0.0, -1.5, 6.0);
		glutSolidCone(1.4, 2.0, 50, 50);
		glPopMatrix();

		glPopMatrix();

		glPopMatrix();

	}
};

#endif
