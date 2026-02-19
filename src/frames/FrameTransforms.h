#pragma once
#include <cmath>
#include "../domain/Vector3D.h"
#include <iostream>
#include "../domain/LL.h"


// ===============================

// Rotation rate of the Earth
constexpr double EARTH_ROT_RATE_RAD_S = 7.2921159e-5;

// Initiate the Matrix 3x3 used for rotation of the vector
struct Matrix3x3 {
    double m[3][3]; 
};

// Creation of the rotation matrix around Z-axis
Matrix3x3 rotationMatrixZ(double angle);

// Performing the matrix-vector product
Vector3D rotateZ(const Matrix3x3 R, const Vector3D v);

LL ecef2ll(Vector3D r);