#include "camera.hpp"

#include "opengl.hpp"

void Camera::apply() {
    double w = glutGet(GLUT_WINDOW_WIDTH), h = glutGet(GLUT_WINDOW_HEIGHT);
    double aspect = w/h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, aspect, 0.01*distance, 100*distance);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -distance);
    glRotatef(latitude, 1, 0, 0);
    glRotatef(-longitude, 0, 1, 0);
    glTranslatef(-target(0), -target(1), -target(2));
}
