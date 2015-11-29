#ifndef WORLD_HPP
#define WORLD_HPP

#include "shape.hpp"
#include <Eigen/Dense>
#include <vector>
using namespace Eigen;

// This is an extremely rudimentary world with just a sphere bouncing around.
// It only exists to demonstrate collision processing with SDFs.

class Sphere {
public:
    Vector3d x, v;
    double r;
    Sphere(Vector3d x, Vector3d v, double r): x(x), v(v), r(r) {}
};

class World {
public:
    std::vector<Sphere*> spheres;  // dynamic sphere(s)
    std::vector<Shape*> obstacles; // static obstacles
    void draw();
    void processCollisions(double dt, double e); // e = coeff. of restitution
    void step(double dt);
};

#endif
