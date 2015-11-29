//
//  main.cpp
//  3D simulations
//
//  Created by LEI NIE on 15/11/28.
//  Copyright © 2015年 LEI NIE. All rights reserved.
//
#include <glew.h>
#include <GLFW/glfw3.h>

#include "camera.hpp"
#include "opengl.hpp"
#include "world.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/cvwimage.h>
#include <opencv/cxcore.h>
//#include <opencv/cxeigen.hpp>
#include <opencv/cxmisc.h>
#include <opencv/ml.h>
using namespace cv;

#ifdef __APPLE__
#include <GL/glut.h>   //For FreeGLUT
#include <OpenGL/gl.h> //OpenGL on Mac OS X http://alumni.cs.ucsb.edu/~wombatty/tutorials/opengl_mac_osx.html
#include <OpenGL/glu.h>
#include <GLUT/glut.h> //Use this line instead for built-in OSX GLUT
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
//#include <GL/freeglut.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <math.h>
using namespace::std;
using namespace::cv;


int w = 800, h = 800; // size of window in pixels
Camera camera;        // camera
double dt = 0.05;     // simulation time step

struct MouseInfo {
    bool dragging;    // is mouse being dragged?
    int xdrag, ydrag; // where the drag began
    int xprev, yprev; // where the mouse was last seen
} mouseInfo;

World world;
//#define FREE_ARG1 char*
int g_rot_abt_x = 0, g_rot_abt_y=0, g_rot_abt_z=0;// variables that take input from keyboard(in glutKeyboardFunc)

void drawCube() {
    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    glColor3f(0.6,0.6,0.6);
    glBegin(GL_LINES);
    double x[] = {-1, 1}, y[] = {0, 2}, z[] = {-1, 1};
    for (int j = 0; j < 2; j++) {
        for (int k = 0; k < 2; k++) {
            glVertex3f(x[0], y[j], z[k]);
            glVertex3f(x[1], y[j], z[k]);
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int k = 0; k < 2; k++) {
            glVertex3f(x[i], y[0], z[k]);
            glVertex3f(x[i], y[1], z[k]);
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            glVertex3f(x[i], y[j], z[0]);
            glVertex3f(x[i], y[j], z[1]);
        }
    }
    glEnd();
    glPopAttrib();
}

void display() {
    glClearColor(0.0,0.5,0.8,1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    glRotatef((GLfloat)g_rot_abt_z, 0.0, 0.0, 1.0);//rotates the visualization about z-axis by an angle given by g_rot_abt_z
    glRotatef((GLfloat)g_rot_abt_y, 0.0, 1.0, 0.0);//rotates the visualization about y-axis by an angle given by g_rot_abt_y
    glRotatef((GLfloat)g_rot_abt_x, 1.0, 0.0, 0.0);//rotates the visualization about x-axis by an angle given by g_rot_abt_x
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_NORMALIZE);
    
    camera.apply();
    drawCube();
    world.draw();
    
    //画坐标轴
    //设置图元颜色
    glColor3f(1.0,0.0,0.0);
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex3f(1.5,0.0,0.0);
    glColor3f(0.0,1.0,0.0);
    glVertex3f(0.0,1.5,0.0);
    glColor3f(0.0,0.0,1.0);
    glVertex3f(0.0,0.0,1.5);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(1.0,0.0,0.0);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(1.5,0.0,0.0);
    glColor3f(0.0,1.0,0.0);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(0.0,1.5,0.0);
    glColor3f(0.0,0.0,1.0);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(0.0,0.0,1.5);
    glEnd();
    
    glColor3f(1.0,1.0,0.0);
    glMatrixMode(GL_MODELVIEW);// set modelview matrix as the current matrix
    glPushMatrix();// puts the current matrix on top of the matrix stack
    glTranslatef(0.5, 0.5, 0.0);// translates the object given by (x,y,z)
    glutSolidSphere(0.2,100,100);
    glPopMatrix();// replaces current matrix with one below it on the stack
    glPushMatrix();
    glTranslatef(-0.5, 0.5, 0.0);// translates the object given by (x,y,z)
    glutSolidSphere(0.2,100,100);
    glPopMatrix();
    glFlush();// ensures drawing commands are executed rather than storing in a buffer
    
    glutSwapBuffers();
    

}

void reshape(int w, int h) {
    ::w = w;
    ::h = h;
    glViewport(0, 0, w, h);
}

void idle() {
    world.step(dt);
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    ::mouseInfo.xprev = x;
    ::mouseInfo.yprev = y;
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            ::mouseInfo.dragging = true;
            ::mouseInfo.xdrag = x;
            ::mouseInfo.ydrag = y;
        } else if (state == GLUT_UP) {
            ::mouseInfo.dragging = false;
        }
    }
}

void motion(int x, int y) {
    camera.longitude -= (x - ::mouseInfo.xprev)*0.2;
    camera.latitude += (y - ::mouseInfo.yprev)*0.2;
    camera.latitude = min(max(camera.latitude, -90.), 90.);
    ::mouseInfo.xprev = x;
    ::mouseInfo.yprev = y;
}

void keyboard(unsigned char key, int x, int y) // get keyboard input to change the camera view
{
    switch(key){
        case 'x' :
            g_rot_abt_x = (g_rot_abt_x + 5)%360; // to change the angle(to rotate about x-axis) with each input/type
            glutPostRedisplay(); // updates and redisplays the updated visualization
            break;
        case 'X' :
            g_rot_abt_x = (g_rot_abt_x - 5)%360; // to change the angle(to rotate about x-axis) with each input/type in opposite direction
            glutPostRedisplay(); // updates and redisplays the updated visualization
            break;
        case 'y' :
            g_rot_abt_y = (g_rot_abt_y + 5)%360; // to change the angle(to rotate about y-axis) with each input/type
            glutPostRedisplay(); // updates and redisplays the updated visualization
            break;
        case 'Y' :
            g_rot_abt_y = (g_rot_abt_y - 5)%360; // to change the angle(to rotate about y-axis) with each input/type in opposite direction
            glutPostRedisplay(); // updates and redisplays the updated visualization
            break;
        case 'z' :
            g_rot_abt_z = (g_rot_abt_z + 5)%360; // to change the angle(to rotate about z-axis) with each input/type
            glutPostRedisplay(); // updates and redisplays the updated visualization
            break;
        case 'Z' :
            g_rot_abt_z = (g_rot_abt_z - 5)%360; // to change the angle(to rotate about z-axis) with each input/type in opposite direction
            glutPostRedisplay(); // updates and redisplays the updated visualization
            break;
    }
}

int main(int argc, char **argv) {
    camera = Camera(Vector3d(0,1,0), 10, 30); // target position, position relative to target, ertical field of view
    //// a basic camera that orbits the target and never rolls
    // add a ball
    world.spheres.push_back(new Sphere(Vector3d(0.5,1.5,0),Vector3d(0,0,0),0.1));
    world.spheres.push_back(new Sphere(Vector3d(0.0,1.8,0),Vector3d(0,0,0),0.1));
                            
    // add the bunny mesh and signed distance function
    MeshShape *bunny = new MeshShape(Vector3d(0,-0.25,0), Quaterniond(), 7,
                                     "bunny2-decimated.obj",
                                     "bunny2-decimated.sdf");
    world.obstacles.push_back(bunny);
    // add the bounding box as six infinite planes
    world.obstacles.push_back(new PlaneShape(Vector3d(1.0,0,0), Vector3d(-1.0,0,0)));
    world.obstacles.push_back(new PlaneShape(Vector3d(-1.0,0,0), Vector3d(1.0,0,0)));
    world.obstacles.push_back(new PlaneShape(Vector3d(0,0,0), Vector3d(0,1.0,0)));
    world.obstacles.push_back(new PlaneShape(Vector3d(0,2,0), Vector3d(0,-1.0,0)));
    world.obstacles.push_back(new PlaneShape(Vector3d(0,0,1.0), Vector3d(0,0,-1.0)));
    world.obstacles.push_back(new PlaneShape(Vector3d(0,0,-1.0), Vector3d(0,0,1.0)));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH|GLUT_MULTISAMPLE);
    glutInitWindowSize(::w, ::h);
    glutCreateWindow("Animation");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);// to call a keyboard function for the current window(to take input from keyboard)
    glutMainLoop();
}

