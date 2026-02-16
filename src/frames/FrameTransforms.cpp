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

LL ecef2ll(Vector3D r) {
    double lon = std::atan(r.y/r.x) * 180 / M_PI; // [deg]
    if (lon > 180) {
        lon = 180 - lon;
    }
    double lat = std::asin(r.z/r.norm()) * 180 / M_PI; // [deg]
    
    return {lat, lon};
}