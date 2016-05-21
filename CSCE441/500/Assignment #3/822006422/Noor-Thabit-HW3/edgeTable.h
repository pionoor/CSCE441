//
//  edgeTable.cpp
//  CSC441-HW3
//
//  Created by Noor Thabit on 2/21/15.
//  Copyright (c) 2015 Noor Thabit. All rights reserved.
//

#include <stdio.h>
#include <list>
#include<math.h>



// Constants for the clip boundaries
#define TOP   0
#define LEFT  1
#define BOTTOM  2
#define RIGHT   3




using namespace std;

class Vertex{
public:
	Vertex() : x(0), y(0){}
	Vertex(int _x, int _y) : x(_x), y(_y){}
	int x;
	int y;
	Vertex(const Vertex& other) :
		x(other.x), y(other.y)
	{}
	void setXY(int _x, int _y){
		x = _x;
		y = _y;
	}
};



class Edge {
public:
	Edge() : maxY(0), minY(0), currentX(0), xIncr(0.0){}
	Edge(int _maxY, int _minY, float _currentX, float _xIncr) : maxY(_maxY), minY(_minY), currentX(_currentX), xIncr(_xIncr){}
	int maxY, minY;
	float xIncr, currentX;

};



class EdgeTable{


private:
	int intersectX;
	int intersectY;

	vector<Edge> edges; //temp used for sorting the edges before inserting them into the actual Edge Table.
	list<Edge> edgeTable[400];
	int maxScanLine;// This will keep track of the max Y point in the polygon
	int minScanLine;

	//sort edge in the Y axies in asscending order
	static bool sortEdgesY(Edge i, Edge j){
		if (i.minY == j.minY){
			return i.currentX < j.currentX;
		}

		else return i.minY < j.minY;
	}

	//sort edge in the X axies in asscending order
	static bool sortEdgesX(Edge i, Edge j){

		return i.currentX < j.currentX;
	}


	void constructEdge(Vertex i, Vertex j){ // constructs an edge from the passed two vertecies

		int maxY = max(i.y, j.y);
		int currentX = (i.y < j.y) ? i.x : j.x;
		int minY = min(i.y, j.y);
		float dy = (j.y - i.y);
		float dx = (j.x - i.x);
		float invSlope = (dy == 0) ? 0 : dx / dy;

		if (dy != 0) // make sure not to push back a horizantal edge
		{
			edges.emplace_back(maxY, minY, currentX, invSlope);
		}

	}


public:
	// Output from the Intersect function

	// clip boundarys
	int clip_x_min;
	int clip_x_max;
	int clip_y_min;
	int clip_y_max;
	vector<Edge> activeEdgeList;
	vector<Vertex> vertices;
	vector<Vertex> temp;

	EdgeTable(int size){
		maxScanLine = 0;
		minScanLine = 0;
	}
	EdgeTable(){
		maxScanLine = 0;
		minScanLine = 0;
	}


	list<Edge>& operator[] (const int nIndex)
	{
		return edgeTable[nIndex];
	}

	int getMinScanPoint(){
		return minScanLine;
	}

	int getMaxScanPoint(){
		return maxScanLine;
	}


	int getMaxYpoint(){
		return maxScanLine;
	}

	int getMinYpoint(){
		return minScanLine;
	}
	void captureVertex(int x, int y){
		vertices.emplace_back(x, y);
		maxScanLine = (maxScanLine < y) ? y : maxScanLine;
	}


	void buildEdgeTable(){

		for (int i = 0; i < vertices.size() - 1; i++)
		{

			constructEdge(vertices[i], vertices[i + 1]);

			if (vertices.size() > 2 && i == vertices.size() - 2) // last vertex
			{
				constructEdge(vertices.back(), vertices[0]);

				break;
			}

		}
		if (!edges.empty()){
			sort(edges.begin(), edges.end(), sortEdgesY);
			minScanLine = edges[0].minY;

			for (int i = 0; i < edges.size(); i++)
			{
				edgeTable[edges[i].minY].push_back(edges[i]);

			}
		}
	}



	//Add edges to Active Edge List from Sorted Edge Table starting at line
	void addEdgesToALE(int line){
		if (!edgeTable[line].empty()){
			for (auto it = edgeTable[line].begin(); it != edgeTable[line].end(); it++) {
				activeEdgeList.push_back(*it);
			}
		}
	}

	//	Remove edges that end at line
	void rmvEdge(int line){
		for (auto it = activeEdgeList.begin(); it != activeEdgeList.end();)
		{
			if (it->maxY == line){
				activeEdgeList.erase(it);
			}
			else {
				++it;
			}
		}
		sort(activeEdgeList.begin(), activeEdgeList.end(), sortEdgesX);

	}

	//Increment x-values on edges in Active Edge List.
	void updateAELXCurr(){
		for (int i = 0; i < activeEdgeList.size(); i++) {
			activeEdgeList[i].currentX += +activeEdgeList[i].xIncr;
		}
		sort(activeEdgeList.begin(), activeEdgeList.end(), sortEdgesX);
	}

	bool emptyAEL(){
		return activeEdgeList.empty();
	}

	void upFrameBuffer(char* fb){

	}

	bool inside(int testX, int testY, int clipBoundary){
		if (clipBoundary == TOP && testY > clip_y_min){
			return true;
		}
		else if (clipBoundary == LEFT && testX > clip_x_min){
			return true;
		}
		else if (clipBoundary == BOTTOM && testY < clip_y_max){
			return true;
		}
		else if (clipBoundary == RIGHT && testX < clip_x_max){
			return true;
		}
		return false;
	}

	void Intersect(int x0, int y0, int x1, int y1, int clipBoundary) {

		int xd = x1 - x0;
		int yd = y1 - y0;

		// vertical line condition
		if (xd == 0 || yd == 0) {
			if (clipBoundary == TOP) {
				intersectX = x0;
				intersectY = clip_y_min;
			}
			else if (clipBoundary == LEFT) {
				// Won't ever happen (line will be entirely inside or outside the edge)
				intersectX = clip_x_min;
				intersectY = y0;
			}
			else if (clipBoundary == BOTTOM) {
				intersectX = x0;
				intersectY = clip_y_max;
			}
			else if (clipBoundary == RIGHT) {
				// Won't ever happen (line will be entirely inside or outside the edge)
				intersectX = clip_x_max;
				intersectY = y0;
			}

			return;
		}

	}
	int SutherlandHodgmanPolygonClip(int clipBoundary) {
		temp.resize(vertices.size());
		int outCount = 0;

		int sx, sy;
		int px, py;

		int j;

		// start with the last vertex in the in array
		sx = vertices.back().x;
		sy = vertices.back().y;

		for (j = 0; j < vertices.size(); j++) {
			px = vertices[j].x;
			py = vertices[j].y;

			if (inside(px, py, clipBoundary)) {
				if (inside(sx, sy, clipBoundary)) {
					temp[outCount].x = px;
					temp[outCount].y = py;
					outCount++;
				}
				else {
					Intersect(sx, sy, px, py, clipBoundary);
					temp[outCount].x = intersectX;
					temp[outCount].y = intersectY;
					outCount++;
					temp[outCount].x = px;
					temp[outCount].y = py;
					outCount++;
				}
			}
			else {
				if (inside(sx, sy, clipBoundary)) {
					Intersect(sx, sy, px, py, clipBoundary);
					temp[outCount].x = intersectX;
					temp[outCount].y = intersectY;
					outCount++;
				}
			}
			sx = px;
			sy = py;
		}

		return outCount;
	}
};