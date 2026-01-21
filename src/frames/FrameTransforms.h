#pragma once
#include <cmath>
#include "../domain/Vector3D.h"

// ===============================

// Let's define the rotation rate of the Earth
constexpr double EARTH_ROT_RATE_RAD_S = 7.2921159e-5;

// Now let's create the rotational matrix to rotate a vector around Z-axis
// like I used to do in uni projects with MATLAB
struct Matrix3x3 {
    double m[3][3]; // m is just a variable
};

inline Matrix3x3 rotationMatrixZ(double angle) {
    double cos = std::cos(angle);
    double sin = std::sin(angle);

    return {{
        {cos, -sin, 0.0},
        {sin, cos, 0.0},
        {0.0, 0.0, 1.0}
    }};
};

// Performing the matrix-vector product
inline Vector3D rotateZ(const Matrix3x3& R, const Vector3D& v){
    return {
        R.m[0][0]*v.x + R.m[0][1]*v.y + R.m[0][2]*v.z,
        R.m[1][0]*v.x + R.m[1][1]*v.y + R.m[1][2]*v.z,
        R.m[2][0]*v.x + R.m[2][1]*v.y + R.m[2][2]*v.z
    };
}
