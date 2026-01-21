#pragma once
#include "FrameTransforms.h"
#include "../domain/TimeUtils.h"

// ===============================

// Arguments will be the position vector in ECI frame, the instant t0 and the final instant t. The last two will be used to compute the theta angle used to perform the rotation.
inline Vector3D eciToEcef(
    const Vector3D& rEci,
    const TimeUTC& t0,
    const TimeUTC t
) {
    double dt = secondsBetween(t0, t);
    double theta = EARTH_ROT_RATE_RAD_S * dt;
    Matrix3x3 Rz = rotationMatrixZ(theta);
    return rotateZ(Rz, rEci);
}