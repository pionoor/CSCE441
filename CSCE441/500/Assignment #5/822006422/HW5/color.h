//
//  color.h
//  Ray Tracer beta
//
//  Created by Noor Thabit on 4/12/15.
//  Copyright (c) 2015 Noor Thabit. All rights reserved.
//

#ifndef _COLOR_H
#define _COLOR_H

class Color {
    double red, green, blue, alpha;
    
public:
    
    Color (): red(0.0), green(0.0), blue(0.0), alpha(0.0){}
    
    Color (double r, double g, double b, double a): red(r), green(g), blue(b), alpha(a) {}
    
    
    // method functions
    double getRed() { return red; }
    double getGreen() { return green; }
    double getBlue() { return blue; }
    double getAlpha() { return alpha; }
    
    void setRed(double r) {  red = r;}
    void setGreen(double g) {  green = g;}
    void setBlue(double b) { blue = b; }
    void setAlpha(double a) { alpha = a; }
    
    void setRGB(double r, double g, double b){
        red = r;
        green = g;
        blue = b;
    }
    
    double brightness() {
        return(red + green + blue)/3;
    }
    
    Color operator*(double scalar) {
        return Color (red*scalar, green*scalar, blue*scalar, alpha);
    }
    
    Color operator+(Color other) {
        return Color (red + other.getRed(), green + other.getGreen(), blue + other.getBlue(), alpha);
    }
    
    Color operator*(Color color) {
        return Color (red*color.getRed(), green*color.getGreen(), blue*color.getBlue(), alpha);
    }
    
    Color colorAverage(Color color) {
        return Color ((red + color.getRed())/2, (green + color.getGreen())/2, (blue + color.getBlue())/2, alpha);
    }
    
    Color clip() {
        double rgb = red + green + blue;
        double light = rgb - 3;
        if (light > 0) {
            red = red + light*(red/rgb);
            green = green + light*(green/rgb);
            blue = blue + light*(blue/rgb);
        }
        if (red > 1) {red = 1;}
        if (green > 1) {green = 1;}
        if (blue > 1) {blue = 1;}
        if (red < 0) {red = 0;}
        if (green < 0) {green = 0;}
        if (blue < 0) {blue = 0;}
        
        return Color (red, green, blue, alpha);
    }
};


#endif