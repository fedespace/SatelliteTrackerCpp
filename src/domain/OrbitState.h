#pragma once
#include "Vector3D.h"
#include "TimeUTC.h"

// ===============================

enum class OrbitClassif { LEO, MEO, GEO, HEO, GTO };

struct OrbitState {
    Vector3D position_km;
    Vector3D velocity_kms;
    TimeUTC UNIXepoch;
    OrbitClassif classification;
    int validityTle; // [min]
};
