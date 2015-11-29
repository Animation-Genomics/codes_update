#include "grid.hpp"

template <typename T>
T& Grid3<T>::get(int i, int j, int k) {
    return values[i+m*(j+n*k)];
}

template <typename T>
void Grid3<T>::assign(T value) {
    values.assign(values.size(), value);
}

// linear interpolation between a (when f = 0) and b (when f = 1)
template <typename T>
T lerp(double f, T a, T b) {
    return a + f*(b - a);
}

template <typename T>
T Grid3<T>::interpolate(Vector3d x) {
    Vector3d index = (x - x0)/dx;
    // integer part of index
    int i = floor(index(0)), j = floor(index(1)), k = floor(index(2));
    // fractional part
    double fi = index(0) - i, fj = index(1) - j, fk = index(2) - k;
    if (i < 0) {
        i = 0; fi = 0;
    } else if (i >= m-1) {
        i = m-2; fi = 1;
    }
    if (j < 0) {
        j = 0; fj = 0;
    } else if (j >= n-1) {
        j = n-2; fj = 1;
    }
    if (k < 0) {
        k = 0; fk = 0;
    } else if (k >= o-1) {
        k = o-2; fk = 1;
    }
    return lerp(fi,
                lerp(fj,
                     lerp(fk, get(i, j, k), get(i, j, k+1)),
                     lerp(fk, get(i, j+1, k), get(i, j+1, k+1))),
                lerp(fj,
                     lerp(fk, get(i+1, j, k), get(i+1, j, k+1)),
                     lerp(fk, get(i+1, j+1, k), get(i+1, j+1, k+1))));
}

template <typename T>
Matrix<T,3,1> Grid3<T>::gradient(Vector3d x) {
    // there are probably more efficient ways to do this,
    // but this is quick and easy.
    return Matrix<T,3,1>(
        interpolate(x+Vector3d(dx/2,0,0)) - interpolate(x-Vector3d(dx/2,0,0)),
        interpolate(x+Vector3d(0,dx/2,0)) - interpolate(x-Vector3d(0,dx/2,0)),
        interpolate(x+Vector3d(0,0,dx/2)) - interpolate(x-Vector3d(0,0,dx/2))
    )/dx;
}

template class Grid3<double>;
