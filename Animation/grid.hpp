#ifndef GRID_HPP
#define GRID_HPP

#include <Eigen/Dense>
#include <vector>

using namespace Eigen;
using namespace std;

template <typename T>
class Grid3 {
public:
    int m, n, o;         // number of grid cells
    Vector3d x0;      // world-space position of cell (0, 0)
    double dx;        // grid cell spacing
    vector<T> values; // grid cell values
    Grid3() {}
    Grid3(int m, int n, int o, Vector3d x0, double dx):
        m(m), n(n), o(o), x0(x0), dx(dx), values(m*n*o) {}
    // value at cell (i, j)
    T& get(int i, int j, int k);
    T& operator()(int i, int j, int k) {
        return get(i, j, k);
    }
    // assign all values
    void assign(T value);
    // interpolated value at world-space position x
    T interpolate(Vector3d x);
    // interpolated finite-difference gradient at world-space position x,
    // returns a Vector3d if the cell type T is double
    Matrix<T,3,1> gradient(Vector3d x);
};

#endif
