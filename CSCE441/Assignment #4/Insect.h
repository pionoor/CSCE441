//
//  Insect.h
//  CSCE441-HW4
//
//  Created by Noor Thabit on 3/20/15.
//  Copyright (c) 2015 Noor Thabit. All rights reserved.
//

#ifndef CSCE441_HW4_Insect_h
#define CSCE441_HW4_Insect_h
#include "Head.h"
#include "Leg.h"
#include "Body.h"

class Insect{

public:

	Head insectHead;
	Body body;

	// Right side
	Leg topR, middleR, bottomR;

	//left side
	Leg topL, middleL, bottomL;



	Insect(GLfloat w, GLfloat h, GLfloat d){

		body.setWHD(w, h, d);

		insectHead.setWHD(w, h, d);
		insectHead.set_XYZ(0.0, 4.0, 1.5);
		topR.setWHD(w, h, d);

		topR.rightSide = true;
		middleR.rightSide = true;
		bottomR.rightSide = true;


		topR.set_XYZ(5.5, 0.5, 1.0);
		middleR.set_XYZ(5.5, -2.0, 1.0);
		bottomR.set_XYZ(5.5, -4.5, 1.0);

		topL.rightSide = false;
		middleL.rightSide = false;
		bottomL.rightSide = false;
		topL.set_XYZ(-5.5, 0.5, 1.0);
		middleL.set_XYZ(-5.5, -2.0, 1.0);
		bottomL.set_XYZ(-5.5, -4.5, 1.0);
	}

	Insect(){

	}

	void moveWholeBodyUp(){
		topR.translateUp();

		middleR.translateUp();

		bottomR.translateUp();

		topL.translateUp();

		middleL.translateUp();

		bottomL.translateUp();

		body.translateUp();
		insectHead.translateUp();

	}

	void moveWholeBodyDown(){
		topR.translateDown();

		middleR.translateDown();

		bottomR.translateDown();

		topL.translateDown();

		middleL.translateDown();

		bottomL.translateDown();

		body.translateDown();
		insectHead.translateDown();

	}

	void moveAllLegsFwdCont(){
		topR.contMoveFwd();

		middleR.contMoveFwd();

		bottomR.contMoveFwd();

		topL.contMoveFwd();

		middleL.contMoveFwd();

		bottomL.contMoveFwd();
		moveWholeBodyUp();
		body.contMoveTail();
		insectHead.bangHead();
	}




	void moveAllLegsBackCont(){
		topR.contMoveBack();

		middleR.contMoveBack();

		bottomR.contMoveBack();

		topL.contMoveBack();

		middleL.contMoveBack();

		bottomL.contMoveBack();

		moveWholeBodyDown();
		body.contMoveTail();
		insectHead.bangHead();
	}

	void moveRightLeges(int direction){
		//1 = forward, 0 = backward
		if (direction == 1) {
			topR.moveLegFwd();

			middleR.moveLegFwd();

			bottomR.moveLegFwd();
		}

		else if (direction == 0) {
			topR.moveLegBack();

			middleR.moveLegBack();

			bottomR.moveLegBack();

		}


	}



	void moveLeftLeges(int direction){
		//1 = forward, 0 = backward
		if (direction == 1) {
			topL.moveLegFwd();

			middleL.moveLegFwd();

			bottomL.moveLegFwd();
		}

		else if (direction == 0) {
			topL.moveLegBack();

			middleL.moveLegBack();

			bottomL.moveLegBack();

		}


	}

	void move(int direction){
		//1 = forward, 0 = backward
		if (direction == 1) {
			if (topR.angle == 25 || topL.angle >= 1) {
				topL.moveLegFwd();

				middleL.moveLegFwd();

				bottomL.moveLegFwd();

				topR.moveLegBack();

				middleR.moveLegBack();

				bottomR.moveLegBack();
			}

			if (topL.angle == 0) {
				topR.moveLegFwd();

				middleR.moveLegFwd();

				bottomR.moveLegFwd();
			}

			if (topL.angle == 25 || topR.angle >= 1) {

			}

		}

		else if (direction == 0) {
			topL.moveLegBack();

			middleL.moveLegBack();

			bottomL.moveLegBack();

		}

	}

	void animate(){
		while (1) {
			moveAllLegsBackCont();
			glutPostRedisplay();

		}
	}

	void display(){

		body.displaySegments();

		topR.display();

		topL.display();

		middleR.display();
		middleL.display();

		bottomR.display();
		bottomL.display();
		insectHead.display();


		body.displayTail();

		/*

		topL.display();
		middleL.display();
		*/
	}
};


#endif
