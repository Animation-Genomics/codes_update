#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <Eigen/Dense>
using namespace Eigen;

class Camera {
public:
    // a basic camera that orbits the target and never rolls
    Vector3d target;                      // target position
    double distance, latitude, longitude; // position relative to target
    double fov;                           // vertical field of view
    Camera() {}
    Camera(Vector3d target, double dist, double fov,
           double lat=0, double lon=0):
        target(target), distance(dist), latitude(lat), longitude(lon),
        fov(fov) {}
    void apply();
};

#endif
