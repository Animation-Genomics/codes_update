#include "world.hpp"
#include "opengl.hpp"
using namespace std;

void World::draw() {
    glColor3f(0.8,0.4,0.4);
    for (int s = 0; s < spheres.size(); s++) {
        Sphere *sphere = spheres[s];
        glPushMatrix();
        glTranslatef(sphere->x(0), sphere->x(1), sphere->x(2));
        glutSolidSphere(sphere->r, 20, 20);
        glPopMatrix();
    }
    glColor3f(0.6,0.6,0.6);
    for (int o = 0; o < obstacles.size(); o++) {
        obstacles[o]->draw();
    }
}

void World::processCollisions(double dt, double e) {
    // I haven't implemented inter-sphere collisions.
    // if you do, then in complex situations like stacking it may take too many
    // iterations to converge, and you may have to add shock propagation
    // (Guendelman et al. 2003) as a final step.
    bool collision;
    do {
        collision = false;
        for (int s = 0; s < spheres.size(); s++) {
            Sphere *sphere = spheres[s];
            for (int o = 0; o < obstacles.size(); o++) {
                Shape *obstacle = obstacles[o];
                if (obstacle->distance(sphere->x + sphere->v*dt) > sphere->r)
                    continue;
                Vector3d n = obstacle->normal(sphere->x);
                if (sphere->v.dot(n) > 0)
                    continue;
                collision = true;
                sphere->v -= (1 + e)*n*n.dot(sphere->v);
            }
        }
    } while (collision);
}

void World::step(double dt) {
    // a roughly Guendelman-style algorithm
    processCollisions(dt, 0.9);
    Vector3d g = Vector3d(0,-1,0);
    for (int s = 0; s < spheres.size(); s++) {
        Sphere *sphere = spheres[s];
        sphere->v += g*dt;
    }
    processCollisions(dt, 0);
    for (int s = 0; s < spheres.size(); s++) {
        Sphere *sphere = spheres[s];
        sphere->x += sphere->v*dt;
    }
}
