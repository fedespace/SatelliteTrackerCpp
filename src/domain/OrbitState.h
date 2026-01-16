#pragma once
#include "Vector3D.h"
#include "TimeUTC.h"

// ===============================

struct OrbitState {
    Vector3D position_km;
    Vector3D velocity_kms;
    TimeUTC UNIXepoch;
};