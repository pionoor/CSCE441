//
//  shape.h
//  Ray Tracer beta
//
//  Created by Noor Thabit on 4/12/15.
//  Copyright (c) 2015 Noor Thabit. All rights reserved.
//

#ifndef _Shape_H
#define _Shape_H

#include "ray.h"
#include "vector.h"
#include "color.h"

class Shape{
public:
    
    Shape() {}
    
    virtual Color getColor () { return Color (0.0, 0.0, 0.0, 0); }
    
    virtual Vector getNormalAt(Vector intersection_position) {
        return Vector(0, 0, 0);
    }
    
    virtual double intersectionAt(Ray ray) {
        return 0;
    }
    
};


///////////////////////////////////////////////////////////////////////


class Sphere : public Shape {
    Vector center;
    double radius;
    Color color;
    
public:
    
    
    Sphere () {
        center = Vector(0,0,0);
        radius = 1.0;
        color = Color(0.5,0.5,0.5, 0);
    }
    
    Sphere (Vector centerValue, double radiusValue, Color colorValue) {
        center = centerValue;
        radius = radiusValue;
        color = colorValue;
    }
    
    // method functions
    Vector getCenter () { return center; }
    double getRadius () { return radius; }
    virtual Color getColor () { return color; }
    
    virtual Vector getNormalAt(Vector point) {
        // normal always points away from the center of a sphere
        Vector normal = (point + (-center)).normalize();
        return normal;
    }
    
    virtual double intersectionAt(Ray ray) {
        
        Vector ray_origin = ray.getRayOrigin();
        double ray_origin_x = ray_origin.getX();
        double ray_origin_y = ray_origin.getY();
        double ray_origin_z = ray_origin.getZ();
        
        Vector ray_direction = ray.getRayDirection();
        double ray_direction_x = ray_direction.getX();
        double ray_direction_y = ray_direction.getY();
        double ray_direction_z = ray_direction.getZ();
        
        Vector sphere_center = center;
        double sphere_center_x = sphere_center.getX();
        double sphere_center_y = sphere_center.getY();
        double sphere_center_z = sphere_center.getZ();
        
        double b = (2*(ray_origin_x - sphere_center_x)*ray_direction_x) + (2*(ray_origin_y - sphere_center_y)*ray_direction_y) + (2*(ray_origin_z - sphere_center_z)*ray_direction_z);
        double c = pow(ray_origin_x - sphere_center_x, 2) + pow(ray_origin_y - sphere_center_y, 2) + pow(ray_origin_z - sphere_center_z, 2) - (radius*radius);
        
        double discriminant = b*b - 4*c;
        
        if (discriminant > 0) {
            
            // the first root
            double root_1 = ((-1*b - sqrt(discriminant))/2) - 0.000001;
            
            if (root_1 > 0) {
                return root_1;
            }
            else {
                double root_2 = ((sqrt(discriminant) - b)/2) - 0.000001;
                return root_2;
            }
        }
        else {
            // the ray missed the sphere
            return -1;
        }
    }
    
};


//////////////////////////////////////////////////////////////////////



class Floor : public Shape {
    Vector normal;
    double distance;
    Color color;
    
public:
    
    Floor () {
        normal = Vector(1,0,0);
        distance = 0;
        color = Color(0.5,0.5,0.5, 0);
    }
    
    Floor (Vector normalValue, double distanceValue, Color colorValue) {
        normal = normalValue;
        distance = distanceValue;
        color = colorValue;
    }
    
    virtual double intersectionAt(Ray ray) {
        Vector ray_direction = ray.getRayDirection();
        
        double a = ray_direction * normal;
        
        if (a == 0) {
            // ray is parallel to the Floor
            return -1;
        }
        else {
            double b = normal *(ray.getRayOrigin() +(normal *(-distance)));
            return -1*b/a;
        }
    }
    
    // method functions
    Vector getNormal () { return normal; }
    double getDistance () { return distance; }
    virtual Color getColor () { return color; }
    
    virtual Vector getNormalAt(Vector point) {
        return normal;
    }
    
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////



    class Triangle : public Shape {
        Vector a, b, c;
        Vector normal;
        double distance;
        Color color;
        
    public:
        
        Triangle () {
            a = Vector(1, 0, 0);
            b = Vector(0, 1, 0);
            c = Vector(0, 0, 1);

            normal = Vector(1,0,0);
            distance = 0;
            color = Color(0.5,0.5,0.5, 0);
        }
        
        Triangle (Vector _a, Vector _b, Vector _c, Color colorValue) {
            a = _a;
            b = _b;
            c = _c;

            normal = getNormal();
            color = colorValue;
        }
        
        virtual double intersectionAt(Ray ray) {
            Vector rayOrigin = ray.getRayOrigin();
            Vector rayDirection = ray.getRayDirection();
            
            double _a = rayDirection * normal;
            
            // ray is parallel to the Floor
            if (_a == 0) {return -1;}

           else {
                double _b = normal *(ray.getRayOrigin() +(normal *(-distance)));
                double distanceFromTri = -1*_b/_a;
                
                double qx = (rayDirection * distanceFromTri).getX() + rayOrigin.getX();
                double qy = (rayDirection * distanceFromTri).getY() + rayOrigin.getY();
                double qz = (rayDirection * distanceFromTri).getZ() + rayOrigin.getZ();
                
                Vector q(qx, qy, qz);
                
                Vector ca(c.getX() - a.getX(), c.getY() - a.getY(), c.getZ() - a.getZ());
                Vector qa(q.getX() - a.getX(), q.getY() - a.getY(), q.getZ() - a.getZ());
                
                double test1 = (ca.cross(qa)) * normal;
                
                
                Vector bc(b.getX() - c.getX(), b.getY() - c.getY(), b.getZ() - c.getZ());
                Vector qc(q.getX() - c.getX(), q.getY() - c.getY(), q.getZ() - c.getZ());
                
                double test2 = (ca.cross(qa)) * normal;
                
                Vector ab(a.getX() - b.getX(), a.getY() - b.getY(), a.getZ() - b.getZ());
                Vector qb(q.getX() - b.getX(), q.getY() - b.getY(), q.getZ() - b.getZ());
                
                double test3 = (ab.cross(qb)) * normal;
                
                if (test1 >= 0 && test2 >= 0 && test3 >= 0) {
                    return -1*_b/_a;

                }
                else return -1;
            }
        }
        
        // method functions
        virtual Color getColor () { return color; }
        
        
        Vector getNormal () {
            Vector ca(c.getX() - a.getX(), c.getY() - a.getY(), c.getZ() - a.getZ());
            Vector ba(b.getX() - a.getX(), b.getY() - a.getY(), b.getZ() - a.getZ());
            normal = ca.cross(ba).normalize();
            return normal;
        }

        virtual Vector getNormalAt(Vector point) {
            return getNormal();
        }
        
        double getFloorDistance () {
            distance = getNormal() * a;
            return distance;
        }

    
};



#endif