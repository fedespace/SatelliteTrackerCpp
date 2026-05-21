// ===============================

#pragma once
#include <cmath> // Used for basic math functions as sqrt

// ===============================

struct Vector3D {
    double x;
    double y;
    double z;

    // Defining the function to get the magnitude of the vector
    double norm() const {
        return std::sqrt(x*x + y*y + z*z);
    }
};