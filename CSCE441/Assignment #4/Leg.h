//
//  Leg.h
//  CSCE441-HW4
//
//  Created by Noor Thabit on 3/20/15.
//  Copyright (c) 2015 Noor Thabit. All rights reserved.
//
#ifndef CSCE441_HW4_Leg_h
#define CSCE441_HW4_Leg_h
#include <gl\glut.h>

class Leg{
public:

	bool rightSide; //true for the right side, false for the left side.
	bool reverse = false;
	GLfloat moveX, moveY = 0;
	GLfloat angle = 0;
	GLfloat translateY = 0;
	GLfloat x, y, z; //coordinates
	GLfloat w, h, d; //width, height, depth.

	Leg(){
		rightSide = false;
	}

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


	void moveLegFwd(){
		if (angle <= 25) {

			angle += 5.0;
			translateY += 0.5;
			if (angle == 25){
				reverse = true;

			}
		}

		display();
	}

	void moveLegBack(){

		if (angle >= -20) {
			angle -= 1.0;
			translateY -= 0.1;
			if (angle == -20){
				reverse = false;

			}
		}

		display();
	}

	void contMoveFwd(){

		if (!reverse) {
			moveLegFwd();
		}
		if (reverse) {
			moveLegBack();

		}
	}

	void contMoveBack(){

		if (!reverse) {
			moveLegFwd();
		}
		if (reverse) {
			moveLegBack();

		}
	}



	void translateUp(){
		moveX += 0.1;
	}

	void translateDown(){
		moveX -= 0.1;
	}


	void display(){
		if (rightSide) {

			glPushMatrix();
			//translate x

			glTranslatef(0, moveX, 0);

			glPushMatrix();
			//twising leg
			glTranslatef(x, y, z);
			glRotated((GLfloat)angle, 0, 0, 1);
			glTranslatef(0, translateY, 0);

			glPushMatrix();
			glColor4f(0.12f, 0.9f, 0.8f, 1.0f);
			glRotated((GLfloat)-110, 0, 1, 0);
			glutSolidCone(0.3, 5.0, 50, 50);
			glPopMatrix();

			//joint
			glPushMatrix();
			glutSolidSphere(0.3, 10, 10);
			glPopMatrix();

			//fore arm
			glPushMatrix();
			glRotated((GLfloat)angle, 1, 0, 0);
			glPushMatrix();
			glColor4f(0.12f, 0.35f, 0.68f, 1.0f);
			glRotated((GLfloat)180, 1, 0, 0);
			glutSolidCone(0.3, 4.0, 50, 50);
			glPopMatrix();

			glPopMatrix();

			glPopMatrix();

			glPopMatrix();


		}
		else if (!rightSide){

			glPushMatrix();
			//translate x

			glTranslatef(0, moveX, 0);


			glPushMatrix();

			glTranslatef(x, y, z);
			glRotated((GLfloat)-angle, 0, 0, 1);
			glTranslatef(0, translateY, 0);

			glPushMatrix();
			glColor4f(0.12f, 0.9f, 0.8f, 1.0f);
			glRotated((GLfloat)110, 0, 1, 0);
			glutSolidCone(0.3, 5.0, 50, 50);
			glPopMatrix();

			//joint
			glPushMatrix();
			glutSolidSphere(0.3, 10, 10);
			glPopMatrix();

			//fore arm
			glPushMatrix();
			glRotated((GLfloat)angle, 1, 0, 0);
			glPushMatrix();
			glColor4f(0.12f, 0.35f, 0.68f, 1.0f);
			glRotated((GLfloat)180, 1, 0, 0);
			glutSolidCone(0.3, 4.0, 50, 50);
			glPopMatrix();

			glPopMatrix();

			glPopMatrix();

			glPopMatrix();


		}

	}
};

#endif
