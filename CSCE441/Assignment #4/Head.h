//
//  Header.h
//  CSCE441-HW4
//
//  Created by Noor Thabit on 3/20/15.
//  Copyright (c) 2015 Noor Thabit. All rights reserved.
//
#ifndef CSCE441_HW4_Header_h
#define CSCE441_HW4_Header_h
#include <gl\glut.h>

class Head{
public:
	GLfloat x, y, z; //coordinates
	GLfloat w, h, d; //width, height, depth.
	GLfloat angle = 0.0;
	GLfloat turnHeadAngle = 0.0;
	GLfloat moveX, moveY = 0;

	bool reverse = false;


	Head(float _w = 0.0, float _h = 0.0, float _d = 0.0) : w(_w), h(_h), d(_d){}

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

	void moveHeadUp(){

		if (angle <= 5) {
			angle += 1;
			if (angle == 5){
				reverse = true;

			}
		}

		display();
	}


	void moveHeadDown(){

		if (angle > 0) {
			angle -= 1;
			if (angle == 0){
				reverse = false;

			}
		}

		display();
	}

	void turnHeadleft(){

		if (turnHeadAngle <= 15) {
			turnHeadAngle += 1;
		}

		display();
	}

	void turnHeadRight(){

		if (turnHeadAngle >= -15) {
			turnHeadAngle -= 1;
		}

		display();
	}

	void bangHead(){

		if (!reverse) {
			moveHeadUp();
		}
		if (reverse) {
			moveHeadDown();

		}
	}

	void translateUp(){
		moveX += 0.1;
	}

	void translateDown(){
		moveX -= 0.1;
	}


	void display()
	{



		glPushMatrix();
		//translate
		glTranslatef(0, moveX, 0);

		glPushMatrix();
		glRotated((GLfloat)angle, 1, 0, 0);
		glRotated((GLfloat)turnHeadAngle, 0, 0, 1);


		glPushMatrix();
		glColor4f(0.6f, 0.9f, 0.8f, 1.0f);
		glTranslatef(x, y, z);
		glScalef(4.0*w, h, 5 * d);
		glutSolidSphere(0.2, 50, 50);
		glPopMatrix();

		//lift eye
		glPushMatrix();
		glColor4f(0.647f, 0.164f, 0.164f, 1.0f);
		glTranslatef(x + 2.0, y + 1.0, z + 1.0);
		glScalef(0.5*w, 0.2*h, d);
		glutSolidSphere(0.2, 50, 50);
		glPopMatrix();

		//right eye
		glPushMatrix();
		glColor4f(0.647f, 0.164f, 0.164f, 1.0f);
		glTranslatef(x - 2.0, y + 1.0, z + 1.0);
		glScalef(0.5*w, 0.2*h, d);
		glutSolidSphere(0.2, 50, 50);
		glPopMatrix();

		glPopMatrix();

		glPopMatrix();






	}

};

#endif
