#include "FrameTransforms.h"
#include <cmath>

Matrix3x3 rotationMatrixZ(double angle) {
    double cos = std::cos(angle);
    double sin = std::sin(angle);

    return {{
        {cos, sin, 0.0},
        {-sin, cos, 0.0},
        {0.0, 0.0, 1.0}
    }};
};

Vector3D rotateZ(const Matrix3x3 R, const Vector3D v){
    return {
        R.m[0][0]*v.x + R.m[0][1]*v.y + R.m[0][2]*v.z,
        R.m[1][0]*v.x + R.m[1][1]*v.y + R.m[1][2]*v.z,
        R.m[2][0]*v.x + R.m[2][1]*v.y + R.m[2][2]*v.z
    };
}

Radec r2radec(const Vector3D rECEF){
    // From "Orbital Mechanics" - Curtis, algorithm 4.1
    // Norm of the vector
    double norm = rECEF.norm();
    // Direction cosines
    double l = rECEF.x/norm;
    double m = rECEF.y/norm;
    double n = rECEF.z/norm;
    // Declination
    double dec = 1/std::sin(n);
    // Right Ascension
    double ra = 1/std::cos(l/std::cos(dec));
    if (m <= 0) {
        ra = 360 - ra;
    };
    return {ra, dec};
}


