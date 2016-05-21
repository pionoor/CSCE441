//
//  ray.h
//  Ray Tracer beta
//
//  Created by Noor Thabit on 4/12/15.
//  Copyright (c) 2015 Noor Thabit. All rights reserved.
//

#ifndef _RAY_H
#define _RAY_H

#include "vector.h"

class Ray {
    Vector origin, direction;
    
public:
    
	Ray() {
		origin = Vector(0, 0, 0);
		direction = Vector(0, 0, 1);
	}

	Ray(Vector _origin, Vector _direction) {
		origin = _origin;
		direction = _direction;
	}
    
    // method functions
    Vector getRayOrigin () { return origin; }
    Vector getRayDirection () { return direction; }
    
};




/////////////////////////////////////////////////////////////////////

class Cam {
    Vector postition, dirction, right, down;
    
public:
    
    Cam () {
        postition = Vector(0,0,0);
        dirction = Vector(0,0,1);
        right = Vector(0,0,0);
        down = Vector(0,0,0);
    }
    
    Cam (Vector pos, Vector dir, Vector right, Vector down) {
        postition = pos;
        dirction = dir;
        right = right;
        down = down;
    }
    
    Vector getPosition () { return postition; }
    Vector direction () { return dirction; }
    Vector getRight () { return right; }
    Vector getDown () { return down; }
    
};


#endif