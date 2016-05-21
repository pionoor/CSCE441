//
//  vector.h
//  Ray Tracer beta
//
//  Created by Noor Thabit on 4/12/15.
//  Copyright (c) 2015 Noor Thabit. All rights reserved.
//


#ifndef _VECTOR_H
#define _VECTOR_H

#include "math.h"

class Vector {
    double x, y, z;
    
public:
    
    Vector () {
        x = 0;
        y = 0;
        z = 0;
    }
    
    Vector (double i, double j, double k) {
        x = i;
        y = j;
        z = k;
    }
    
    // method functions
    double getX() { return x; }
    double getY() { return y; }
    double getZ() { return z; }
    
    double magnitude () {
        return sqrt((x*x) + (y*y) + (z*z));
    }
    
    Vector normalize () {
        double magnitude = sqrt((x*x) + (y*y) + (z*z));
        return Vector (x/magnitude, y/magnitude, z/magnitude);
    }
    

    
    Vector operator-() {
        return Vector (-x, -y, -z);
    }
    
    //dot product
    double operator*(Vector v) {
        return x*v.getX() + y*v.getY() + z*v.getZ();
    }
    
    Vector cross(Vector v) {
        return Vector (y*v.getZ() - z*v.getY(), z*v.getX() - x*v.getZ(), x*v.getY() - y*v.getX());
    }

    
    friend Vector operator+ (Vector v, Vector other) {
        return Vector (other.x + v.getX(), other.y + v.getY(), other.z + v.getZ());
    }
    
  //scalar
    Vector operator* (double scalar) {
        return Vector (x*scalar, y*scalar, z*scalar);
    }
    
};



#endif