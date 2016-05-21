//
//  main.cpp
//  Ray Tracer beta
//
//  Created by Noor Thabit on 4/12/15.
//  Copyright (c) 2015 Noor Thabit. All rights reserved.
//
#include <vector>
#include <cmath>

#include "ray.h"
#include "color.h"
#include "light.h"
#include "shapes.h"
#include <algorithm>
#include <gl/glut.h>

using namespace std;

#define ImageW 640
#define ImageH 480
#define smallNum 0.0000001
#define ambientLight 0.15
#define depth 1

float framebuffer[ImageH][ImageW][3];
double aspRatio = (double)ImageW/(double)ImageH;


// Draws the scene
void drawit(void)
{
    glDrawPixels(ImageW, ImageH, GL_RGB, GL_FLOAT, framebuffer);
    glFlush();
}

// Clears framebuffer to black
void clearFramebuffer()
{
    
    for(int i=0;i<ImageH;i++) {
        for (int j=0;j<ImageW;j++) {
            framebuffer[i][j][0] = 0.0;
            framebuffer[i][j][1] = 0.0;
            framebuffer[i][j][2] = 0.0;
        }
    }
}

// Sets pixel x,y to the color RGB
void setFramebuffer(int x, int y, float R, float G, float B)
{
    if (R<=1.0)
        if (R>=0.0)
            framebuffer[x][y][0]=R;
        else
            framebuffer[x][y][0]=0.0;
        else
            framebuffer[x][y][0]=1.0;
    if (G<=1.0)
        if (G>=0.0)
            framebuffer[x][y][1]=G;
        else
            framebuffer[x][y][1]=0.0;
        else
            framebuffer[x][y][1]=1.0;
    if (B<=1.0)
        if (B>=0.0)
            framebuffer[x][y][2]=B;
        else
            framebuffer[x][y][2]=0.0;
        else
            framebuffer[x][y][2]=1.0;
}

void init(void)
{
    clearFramebuffer();
}



int getClosestObjIndex(vector<double> intersections) {
    int closestObj = 0;
    //no intersections
    if (intersections.empty()) { return -1; }
   
    else if (intersections.size() == 1) {
        
        if (intersections[0] > 0) { return 0; }
        else { return -1; }
    }
    else {
        double max = *max_element(std::begin(intersections), std::end(intersections));
       
        if (max > 0) {
            for (int i = 0; i < intersections.size(); i++) {
                if (intersections[i] > 0 && intersections[i] <= max) {
                    max = intersections[i];
                    closestObj = i;
                }
            }
            
            return closestObj;
        }
        else { return -1; }
    }
}

Color getIntersectionColor(Vector intersectionPosition, Vector intersectRayDirection, vector<Shape*> shapes, vector<Light> lightSources, int closestObjIndex) {
    
    Color ClosestObjColor = shapes[closestObjIndex]->getColor();
    Vector closestObjNormal = shapes[closestObjIndex]->getNormalAt(intersectionPosition);
    
    ClosestObjColor.setRGB(ClosestObjColor.getRed(), ClosestObjColor.getGreen(), ClosestObjColor.getBlue());

    
    Color returnedColor = ClosestObjColor * ambientLight ;
    
    if (ClosestObjColor.getAlpha() > 0 && ClosestObjColor.getAlpha() <= 1) {
        // reflection
        double dot1 = closestObjNormal * (-intersectRayDirection);
        Vector scalar1 = closestObjNormal * dot1;
        Vector add1 = scalar1 + intersectRayDirection;
        Vector scalar2 = add1 * 2 ;
        Vector add2 = (-intersectRayDirection) + scalar2;
        Vector refDirection = add2.normalize();
        
        Ray reflection_ray (intersectionPosition, refDirection);
        
        // determine what the ray intersects with first
        vector<double> reflection_intersections;
        
        for (int reflection_index = 0; reflection_index < shapes.size(); reflection_index++) {
            reflection_intersections.push_back(shapes[reflection_index]->intersectionAt(reflection_ray));
        }
        
        int closestIndexObjRef = getClosestObjIndex(reflection_intersections);
        
        if (closestIndexObjRef != -1) {
            if (reflection_intersections[closestIndexObjRef] > smallNum) {
    
                
                Vector refIntersectionPosition = intersectionPosition +(refDirection * reflection_intersections[closestIndexObjRef]);
                Vector refIntersectionRayDir = refDirection;
                
                Color refIntersectionColor = getIntersectionColor(refIntersectionPosition, refIntersectionRayDir, shapes, lightSources, closestIndexObjRef);
                
                returnedColor = returnedColor + refIntersectionColor * (ClosestObjColor.getAlpha());
            }
        }
    }
    
    for (int i = 0; i < lightSources.size(); i++) {
        Vector lightDirection = (lightSources[i].getLightPosition() + (-intersectionPosition)).normalize();
        
        float cosAngle = closestObjNormal * lightDirection;
        
        if (cosAngle > 0) {
            bool isShadowed = false;
            
            Vector distFromlight = (lightSources[i].getLightPosition() + (-intersectionPosition)).normalize();
            float distFromlightMag = distFromlight.magnitude();
            
            Ray shadowRay (intersectionPosition, (lightSources[i].getLightPosition() + (-intersectionPosition)).normalize());
            
            vector<double> secondary_intersections;
            
            for (int i = 0; i < shapes.size() && isShadowed == false; i++) {
                secondary_intersections.push_back(shapes[i]->intersectionAt(shadowRay));
            }
            
            for (int j = 0; j < secondary_intersections.size(); j++) {
                if (secondary_intersections[j] > smallNum) {
                    if (secondary_intersections[j] <= distFromlightMag) {
                        isShadowed = true;
                    }
                    break;
                }
            }
            
            if (isShadowed == false) {
                returnedColor = returnedColor + ClosestObjColor * lightSources[i].getLightColor() * cosAngle ;
                
                if (ClosestObjColor.getAlpha() > 0 && ClosestObjColor.getAlpha() <= 1) {
                    double dot1 = closestObjNormal * (-intersectRayDirection);
                    Vector scalar1 = closestObjNormal * dot1;
                    Vector add1 = scalar1 + intersectRayDirection;
                    Vector scalar2 = add1 * 2 ;
                    Vector add2 = (-intersectRayDirection) + scalar2;
                    Vector refDirection = add2.normalize();
                    
                    double specular = refDirection * lightDirection;
                    if (specular > 0) {
                        specular = pow(specular, 10);
                        returnedColor = returnedColor + lightSources[i].getLightColor() * (specular*ClosestObjColor.getAlpha());
                    }
                }
            }
        }
    }
    
    return returnedColor.clip();
}

void display(void)
{
 
    Vector camPosition (3, 1.5, -4);
    
    Vector diffrence (camPosition.getX() - Vector(0, 0, 0).getX(), camPosition.getY() - Vector(0, 0, 0).getY(), camPosition.getZ() - Vector(0, 0, 0).getZ());
    
    Vector camDirection = (-diffrence).normalize();
    Vector camRight = Vector(0,1,0).cross(camDirection).normalize();
    Vector camDown = camRight.cross(camDirection);
    Cam sceneCam (camPosition, camDirection, camRight, camDown);
    
    //lights
    vector<Light> lightSources;
    lightSources.push_back(Light(Vector (-7, 10,-10), Color(1.0, 1.0, 1.0, 0)));
    lightSources.push_back(Light(Vector (-2, 5, 10), Color(0.5, 0.5, 0.5, 0)));

    
    //scene Shapes
    vector<Shape*> Shapes;

    Shapes.push_back(dynamic_cast<Shape*>(new Sphere (Vector(-0.5,0,0), 0.85, Color(0.77, 0.1, 0.5, 0.3))));
    Shapes.push_back(dynamic_cast<Shape*>(new Sphere (Vector(-3.75, -0.25, 0), 0.5, Color(0.5, 0.25, 0.25, 0))));
    Shapes.push_back(dynamic_cast<Shape*>(new Sphere (Vector(-3.75, 1, 3), 0.5, Color(0.15, 0.45, 0.55, 0.4))));
    Shapes.push_back(dynamic_cast<Shape*>(new class Floor(Vector(0,1,0), -1, Color(1, 1, 1, 2))));
    Shapes.push_back(dynamic_cast<Shape*>(new Triangle(Vector(0,0,1), Vector(1,0,0), Vector(0,0,5), Color(0.5, 0.5, 0.5, 0)
)));

    Shapes.push_back(dynamic_cast<Shape*>(new Triangle(Vector(0,0,2), Vector(1,1,0), Vector(1,2,3), Color(0.54, 0.15, 0.95, 0.3)
                                                       )));
  
    
    double xAmount, yAmount;
    
    for (int x = 0; x < ImageW; x++) {
        for (int y = 0; y < ImageH; y++) {
            
            double tempRed[depth*depth];
            double tempGreen[depth*depth];
            double tempBlue[depth*depth];
            
            for (int aax = 0; aax < depth; aax++) {
                for (int aay = 0; aay < depth; aay++) {
                    
                    
                    if (depth == 1) {
                        
                        // start with no anti-aliasing
                        if (ImageW > ImageH) {
                            // the image is wider than it is tall
                            xAmount = ((x+0.5)/ImageW)*aspRatio - (((ImageW-ImageH)/(double)ImageH)/2);
                            yAmount = ((ImageH - y) + 0.5)/ImageH;
                        }
                        else if (ImageH > ImageW) {
                            // the imager is taller than it is wide
                            xAmount = (x + 0.5)/ ImageW;
                            yAmount = (((ImageH - y) + 0.5)/ImageH)/aspRatio - (((ImageH - ImageW)/(double)ImageW)/2);
                        }
                        else {
                            // the image is square
                            xAmount = (x + 0.5)/ImageW;
                            yAmount = ((ImageH - y) + 0.5)/ImageH;
                        }
                    }
                    else {
                        // anti-aliasing
                        if (ImageW > ImageH) {
                            // the image is wider than it is tall
                            xAmount = ((x + (double)aax/((double)depth - 1))/ImageW)*aspRatio - (((ImageW-ImageH)/(double)ImageH)/2);
                            yAmount = ((ImageH - y) + (double)aax/((double)depth - 1))/ImageH;
                        }
                        else if (ImageH > ImageW) {
                            // the imager is taller than it is wide
                            xAmount = (x + (double)aax/((double)depth - 1))/ ImageW;
                            yAmount = (((ImageH - y) + (double)aax/((double)depth - 1))/ImageH)/aspRatio - (((ImageH - ImageW)/(double)ImageW)/2);
                        }
                        else {
                            // the image is square
                            xAmount = (x + (double)aax/((double)depth - 1))/ImageW;
                            yAmount = ((ImageH - y) + (double)aax/((double)depth - 1))/ImageH;
                        }
                    }
                    
                    Vector camRayOrigin = sceneCam.getPosition();
                    Vector camRayDirection = (camDirection + (camRight * (xAmount - 0.5)) +(camDown *(yAmount - 0.5))).normalize();
                    
                    Ray camRay (camRayOrigin, camRayDirection);
                    
                    vector<double> intersections;
                    
                    for (int i = 0; i < Shapes.size(); i++) {
                        intersections.push_back(Shapes[i]->intersectionAt(camRay));
                    }
                    
                    int closestObjIndex = getClosestObjIndex(intersections);
                    
                    if (closestObjIndex == -1) {
                        // set the backgroung black
                        tempRed[aay*depth + aax] = 0;
                        tempGreen[aay*depth + aax] = 0;
                        tempBlue[aay*depth + aax] = 0;
                    }
                    else{
                        if (intersections[closestObjIndex] > smallNum) {
                            
                            Vector intersectionPosition = camRayOrigin + (camRayDirection * intersections[closestObjIndex]);
                            Vector intersecting_ray_direction = camRayDirection;
                            
                            Color colorAtIntersection = getIntersectionColor(intersectionPosition, intersecting_ray_direction, Shapes, lightSources, closestObjIndex);
                            
                            tempRed[aay*depth + aax] = colorAtIntersection.getRed();
                            tempGreen[aay*depth + aax] = colorAtIntersection.getGreen();
                            tempBlue[aay*depth + aax] = colorAtIntersection.getBlue();
                        }
                    }
                }
            }
            
            double red = 0;
            double green = 0;
            double yellew = 0;
            
            for (int r = 0; r < depth*depth; r++) {
                red = red + tempRed[r];
            }
            for (int g = 0; g < depth*depth; g++) {
                green = green + tempGreen[g];
            }
            for (int b = 0; b < depth*depth; b++) {
                yellew = yellew + tempBlue[b];
            }
            
            double avgRed = red/(depth*depth);
            double avgGreen = green/(depth*depth);
            double avgBlue = yellew/(depth*depth);
            
            setFramebuffer(y, x, avgRed, avgGreen, avgBlue);
        }
    }
    
    drawit();
}



int main (int argc, char *argv[]) {
    
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(ImageW,ImageH);
    glutInitWindowPosition(100,100);
    glutCreateWindow("<Noor Thabit> - Homework 3");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}