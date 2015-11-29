#include "mesh.hpp"
#include "opengl.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

Face::Face(Vertex *v0, Vertex *v1, Vertex *v2) {
    vertices[0] = v0;
    vertices[1] = v1;
    vertices[2] = v2;
    n = (v1->x - v0->x).cross(v2->x - v0->x).normalized();
}

// angle at x0 of triangle with vertices x0, x1, x2
double angle(const Vector3d &x0, const Vector3d &x1, const Vector3d &x2) {
    return acos((x1 - x0).normalized().dot((x2 - x0).normalized()));
}

// convert a polygon with given vertices into collection of triangles
vector<Face*> triangulate (const vector<Vertex*> &vertices) {
    int n = vertices.size();
    double best_min_angle = 0;
    int best_root = -1;
    for (int i = 0; i < n; i++) {
        double min_angle = 1e6;
        const Vertex *vertex0 = vertices[i];
        for (int j = 2; j < n; j++) {
            const Vertex *vertex1 = vertices[(i+j-1)%n];
            const Vertex *vertex2 = vertices[(i+j)%n];
            double angle0 = angle(vertex0->x, vertex1->x, vertex2->x);
            double angle1 = angle(vertex1->x, vertex2->x, vertex0->x);
            double angle2 = angle(vertex2->x, vertex0->x, vertex1->x);
            min_angle = min(min(min_angle, angle0), min(angle1, angle2));
        }
        if (min_angle > best_min_angle) {
            best_min_angle = min_angle;
            best_root = i;
        }
    }
    int i = best_root;
    Vertex* vertex0 = vertices[i];
    vector<Face*> triangles;
    for (int j = 2; j < n; j++) {
        Vertex *vertex1 = vertices[(i+j-1)%n], *vertex2 = vertices[(i+j)%n];
        triangles.push_back(new Face(vertex0, vertex1, vertex2));
    }
    return triangles;
}

void Mesh::readObj(string filename) {
    vertices.clear();
    faces.clear();
    fstream file(filename.c_str(), ios::in);
    if(!file) {
        cerr << "Error: failed to open file " << filename << endl;
        return;
    }
    while (file) {
        string line;
        do
            getline(file, line);
        while (file && (line.length() == 0 || line[0] == '#'));
        stringstream linestream(line);
        string keyword;
        linestream >> keyword;
        if (keyword == "v") {
            // vertex
            Vector3d x;
            linestream >> x[0] >> x[1] >> x[2];
            vertices.push_back(new Vertex(x));
        } else if (keyword == "f") {
            // face(s)
            vector<Vertex*> polygon;
            string w;
            while (linestream >> w) {
                stringstream wstream(w);
                int v;
                wstream >> v;
                polygon.push_back(vertices[v-1]);
            }
            vector<Face*> triangles = triangulate(polygon);
            faces.insert(faces.end(), triangles.begin(), triangles.end());
        }
    }
}

void Mesh::draw() {
    glBegin(GL_TRIANGLES);
    for (int f = 0; f < faces.size(); f++) {
        Face *face = faces[f];
        glNormal3d(face->n(0), face->n(1), face->n(2));
        for (int v = 0; v < 3; v++) {
            Vertex *vertex = face->vertices[v];
            glVertex3d(vertex->x(0), vertex->x(1), vertex->x(2));
        }
    }
    glEnd();
}
