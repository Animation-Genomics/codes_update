#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "grid.hpp"
#include "mesh.hpp"
#include <Eigen/Dense>
using namespace Eigen;

// Shape only represents collision geometry.
// if you wanted to do rigid body dynamics, you should create a new class
// RigidBody that contains a Shape as well as the dynamical information
// (mass, moment of inertia, world-space position and orientation,
// linear velocity, angular momentum).

class Shape {
public:
    virtual void draw() = 0;
    virtual double distance(Vector3d x) = 0; // signed distance to surface
    virtual Vector3d normal(Vector3d x) = 0; // normal at the position x
};

// just an infinite plane
class PlaneShape: public Shape {
public:
    Vector3d x, n; // point on plane, plane normal
    PlaneShape(Vector3d x, Vector3d n): x(x), n(n) {}
    virtual void draw() {}
    virtual double distance(Vector3d x1) {return (x1 - x).dot(n);}
    virtual Vector3d normal(Vector3d x1) {return n;}
};

// a triangle mesh and a precomputed signed distance function
class MeshShape: public Shape {
public:
    Vector3d x;    // position
    Quaterniond q; // orientation
    double scale;  // uniform scaling
    Mesh mesh;         
    Grid3<double> sdf;
    MeshShape() {};
    MeshShape(Vector3d x, Quaterniond q, double scale,
              std::string meshfile, std::string sdffile);
    virtual void draw();
    virtual double distance(Vector3d x);
    virtual Vector3d normal(Vector3d x);
};

#endif
