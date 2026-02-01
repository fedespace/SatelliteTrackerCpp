#pragma once
#include "FrameTransforms.h"
#include "../domain/TimeUtils.h"

// ===============================

// Arguments will be the position vector in TEME frame, the instant t0 and the final instant t. The last two will be used to compute the theta angle used to perform the rotation.
inline Vector3D TEME2ECEF(
    const Vector3D& rTeme,
    // const TimeUTC& t0,
    const TimeUTC t
) {
    // Compute using the function about MJD2000
    double minSinceMJD = epoch2MJD2000(t);
    double dt = minSinceMJD*60;
    // double dt = secondsBetween(t0, t);
    double theta = EARTH_ROT_RATE_RAD_S * dt;
    // Minus, because we are not rotating the satellite but the coordinate frames which is equal to rotate the Earth eastwards.
    Matrix3x3 Rz = rotationMatrixZ(-theta);
    return rotateZ(Rz, rTeme);
}