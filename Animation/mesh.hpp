#ifndef MESH_HPP
#define MESH_HPP

#include <Eigen/Dense>
#include <string>
#include <vector>
using namespace Eigen;

// For simplicity, Mesh only represents triangle meshes.
// Polygons with more than 3 vertices will be triangulated on reading.

class Vertex {
public:
    Vector3d x;
    Vertex(Vector3d x): x(x) {}
};

class Face {
public:
    Vertex *vertices[3];
    Vector3d n; // normal
    Face(Vertex *v0, Vertex *v1, Vertex *v2);
};

class Mesh {
public:
    std::vector<Vertex*> vertices;
    std::vector<Face*> faces;
    void readObj(std::string filename); // read an OBJ file
    void draw();
};

#endif
