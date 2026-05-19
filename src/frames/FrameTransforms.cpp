#include "FrameTransforms.h"
#include <cmath>

Matrix3x3 rotationMatrixZ(double angle) {
    double cos = std::cos(angle);
    double sin = std::sin(angle);

    return {{
        {cos, -sin, 0.0},
        {sin, cos, 0.0},
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

void ecef2ll(Vector3D r, double rE, double& lat, double& lon) {
    lon = std::atan2(r.y,r.x) * 180 / M_PI; // [deg]
    if (lon > 180) {
        lon -= 360;
    }

    // Definition of var for iteration
    double p = std::sqrt(r.x*r.x + r.y*r.y); // distance from Earth's rotation axis
    double lat_0 = atan2(r.z, p);
    lat = lat_0 + 10000;
    double earth_ecc_squared = 0.006694385;

    // Loop
    while (std::abs(lat - lat_0) > 1e-10) {
        lat_0 = lat;
        double N = rE / (std::sqrt(1 - earth_ecc_squared * std::pow(std::sin(lat),2)));
        lat = std::atan2(r.z + earth_ecc_squared * N * std::sin(lat), p);
    }

    lat = lat * 180 / M_PI;

    // if (lon > 180) {
    //     lon = 180 - lon;
    // }
    // lat = std::asin(r.z/r.norm()) * 180 / M_PI; // [deg]

}