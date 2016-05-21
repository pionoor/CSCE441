//
//  main.cpp
//  CSC441-HW3
//
//  Created by Noor Thabit on 2/21/15.
//  Copyright (c) 2015 Noor Thabit. All rights reserved.
//

#include<list>
#include<vector>
#include<gl/GLUT.h>
#include<cmath>
#include <algorithm>
#include"edgeTable.h"
#include<cstdlib>

/******************************************************************
Notes:
Image size is 400 by 400 by default.  You may adjust this if
you want to.
You can assume the window will NOT be resized.
Call clearFramebuffer to clear the entire framebuffer.
Call setFramebuffer to set a pixel.  This should be the only
routine you use to set the color (other than clearing the
entire framebuffer).  drawit() will cause the current
framebuffer to be displayed.
As is, your scan conversion should probably be called from
within the display function.  There is a very short sample
of code there now.
You may add code to any of the subroutines here,  You probably
want to leave the drawit, clearFramebuffer, and
setFramebuffer commands alone, though.
*****************************************************************/
using namespace std;

#define ImageW 400
#define ImageH 400

float framebuffer[ImageH][ImageW][3];
vector<EdgeTable> polyEdgeTables(1);
vector<EdgeTable> ClipedpolyEdgeTables(1);

Vertex vertLineStart(0, 0), vertLineEnd(0, 0);
bool clipMode = false;
Vertex clipRecEdges[2];
float r, g, b = 1.0;
bool lastVert = false;
int start_x;
int start_y;

class Color {
	float r, g, b;		// Color (R,G,B values)
};


void changeColor(){
	r = (double)(rand() % 255) / 255;
	g = (double)(rand() % 255) / 255;
	b = (double)(rand() % 255) / 255;

}


// Draws the scene
void drawit(void)
{
	glDrawPixels(ImageW, ImageH, GL_RGB, GL_FLOAT, framebuffer);
	glFlush();
}

// Clears framebuffer to black
void clearFramebuffer()
{
	int i, j;

	for (i = 0; i<ImageH; i++) {
		for (j = 0; j<ImageW; j++) {
			framebuffer[i][j][0] = 0.0;
			framebuffer[i][j][1] = 0.0;
			framebuffer[i][j][2] = 0.0;
		}
	}
}


// Sets pixel x,y to the color RGB
// I've made a small change to this function to make the pixels match
// those returned by the glutMouseFunc exactly - Scott Schaefer

void setFramebuffer(int x, int y, float R, float G, float B)
{
	// changes the origin from the lower-left corner to the upper-left corner
	y = ImageH - 1 - y;
	if (R <= 1.0)
		if (R >= 0.0)
			framebuffer[y][x][0] = R;
		else
			framebuffer[y][x][0] = 0.0;
	else
		framebuffer[y][x][0] = 1.0;
	if (G <= 1.0)
		if (G >= 0.0)
			framebuffer[y][x][1] = G;
		else
			framebuffer[y][x][1] = 0.0;
	else
		framebuffer[y][x][1] = 1.0;
	if (B <= 1.0)
		if (B >= 0.0)
			framebuffer[y][x][2] = B;
		else
			framebuffer[y][x][2] = 0.0;
	else
		framebuffer[y][x][2] = 1.0;
}



void drawPoly(){

	for (int line = polyEdgeTables.back().getMinScanPoint(); line <= polyEdgeTables.back().getMaxScanPoint(); line++) {

		//	Add edges to Active Edge List from Sorted Edge 	Table starting at line
		polyEdgeTables.back().addEdgesToALE(line);

		//purge edges from AEL for which maxY = y, and sort edges in order of increasing xcurrent values
		polyEdgeTables.back().rmvEdge(line);

		if (polyEdgeTables.back().emptyAEL()) {
			break;
		}

		//plot pixels along scan line between pairs of edges
		for (int i = 0; i < polyEdgeTables.back().activeEdgeList.size() - 1; i += 2) {
			int x0 = ceil(polyEdgeTables.back().activeEdgeList[i].currentX);
			int xEnd = floor(polyEdgeTables.back().activeEdgeList[i + 1].currentX);
			//fillå
			for (int j = x0; j <= xEnd; j++) {
				setFramebuffer(j, line, r, g, b);
			}
		}
		//update each xcurrent value in AEL
		polyEdgeTables.back().updateAELXCurr();
	}
	glutPostRedisplay();
	//drawit();
}





void drawClipedPoly(){
	for (int line = ClipedpolyEdgeTables.back().getMinScanPoint(); line <= ClipedpolyEdgeTables.size(); line++) {

		//	Add edges to Active Edge List from Sorted Edge 	Table starting at line
		ClipedpolyEdgeTables.back().addEdgesToALE(line);

		//purge edges from AEL for which maxY = y, and sort edges in order of increasing xcurrent values
		ClipedpolyEdgeTables.back().rmvEdge(line);

		if (ClipedpolyEdgeTables.back().emptyAEL()) {
			break;
		}

		//plot pixels along scan line between pairs of edges
		for (int i = 0; i < ClipedpolyEdgeTables.back().activeEdgeList.size() - 1; i += 2) {
			int x0 = ceil(ClipedpolyEdgeTables.back().activeEdgeList[i].currentX);
			int xEnd = floor(ClipedpolyEdgeTables.back().activeEdgeList[i + 1].currentX);
			//fillå
			for (int j = x0; j <= xEnd; j++) {
				setFramebuffer(j, line, r, g, b);
			}
		}
		//update each xcurrent value in AEL
		ClipedpolyEdgeTables.back().updateAELXCurr();
	}

	glutPostRedisplay();
}

void drawEdgeLines(){
	glColor3f(r, g, b);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < polyEdgeTables.back().vertices.size(); i++) {
		GLfloat x = polyEdgeTables.back().vertices[i].x;
		GLfloat y = polyEdgeTables.back().vertices[i].y;

		glVertex2f(x, y);
	}
	glutPostRedisplay();

}

void drawClipingRec(){

	glColor3f(1.0, 1.0, 1.0);

	if (clipMode) {
		glBegin(GL_LINE_LOOP);

		glVertex2d(clipRecEdges[0].x, clipRecEdges[0].y);
		glVertex2i(clipRecEdges[1].x, clipRecEdges[0].y);
		glVertex2i(clipRecEdges[1].x, clipRecEdges[1].y);
		glVertex2i(clipRecEdges[0].x, clipRecEdges[1].y);

	}

	glEnd();
	glFlush();
}

void clip() {
	ClipedpolyEdgeTables.resize(polyEdgeTables.size());
	for (int i = 0; i < polyEdgeTables.size(); i++) {

		ClipedpolyEdgeTables[i].vertices = polyEdgeTables[i].vertices;
		ClipedpolyEdgeTables[i].clip_x_min = clipRecEdges[0].x;
		ClipedpolyEdgeTables[i].clip_y_min = clipRecEdges[0].y;
		ClipedpolyEdgeTables[i].clip_x_max = clipRecEdges[1].x;
		ClipedpolyEdgeTables[i].clip_y_max = clipRecEdges[1].y;

		ClipedpolyEdgeTables[i].SutherlandHodgmanPolygonClip(0); // top
		ClipedpolyEdgeTables[i].SutherlandHodgmanPolygonClip(1); // right

		ClipedpolyEdgeTables[i].SutherlandHodgmanPolygonClip(2); // left
		ClipedpolyEdgeTables[i].SutherlandHodgmanPolygonClip(3); //down
		ClipedpolyEdgeTables[i].vertices = ClipedpolyEdgeTables[i].temp;
		ClipedpolyEdgeTables[i].buildEdgeTable();
		drawClipedPoly();

	}



}

void display(void)
{
	drawit();
	//glClear(GL_COLOR_BUFFER_BIT);
	drawClipingRec();
}

void keyboard(unsigned char key, int x, int y){
	if (key == 'c') {
		clipMode = true;
	}
}

void mouse(int button, int state, int x, int y)
{

	start_x = x;
	start_y = y;
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN){
			if (!clipMode) {
				if (lastVert) {
					polyEdgeTables.emplace_back();
					lastVert = false;
				}
				polyEdgeTables.back().captureVertex(x, y);
				drawEdgeLines();

			}

			if (clipMode) {
				clipRecEdges[0].x = x;
				clipRecEdges[0].y = y;

			}


		}
		else {
			if (clipMode) {
				clipRecEdges[1].x = x;
				clipRecEdges[1].y = y;
				clip();
			}



		}
		break;

	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN){
			if (!clipMode) {

				polyEdgeTables.back().captureVertex(x, y);
				polyEdgeTables.back().buildEdgeTable();
				drawPoly();
				changeColor();
				lastVert = true;


			}




		}
		else {

		}

		break;

	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN){
		}
		else{
		}
		break;
	default:
		break;
	}
}


void motion(int x, int y){
	clipRecEdges[1].x = x;
	clipRecEdges[1].y = y;

	if (clipMode) {
	}
	glutPostRedisplay();
	start_x = x;
	start_y = y;
}



void init(void)
{
	gluOrtho2D(0.0, ImageW - 1, ImageH - 1, 0.0);
	clearFramebuffer();
}

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(ImageW, ImageH);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("<Noor Thabit> - Homework 3");
	init();
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}