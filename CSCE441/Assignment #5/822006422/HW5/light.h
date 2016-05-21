//
//  ligh.h
//  Ray Tracer beta
//
//  Created by Noor Thabit on 4/12/15.
//  Copyright (c) 2015 Noor Thabit. All rights reserved.
//

#ifndef _Light_H
#define _Light_H

#include "color.h"

class Light {
    Vector position;
    Color color;
    
public:
    
    Light () {
        position = Vector(0,0,0);
        color = Color(1,1,1, 0);
    }
    
    Light (Vector p, Color c) {
        position = p;
        color = c;
    }
    
    // method functions
    virtual Vector getLightPosition () { return position; }
    virtual Color getLightColor () { return color; }
    
};


#endif