#pragma once
#include "../domain/Tle.h"
#include "../domain/OrbitState.h"
#include "../domain/TimeUTC.h"

// ===============================

class TlePropagator {
public:
    // Let's create the destructor using the tilde sign
    virtual ~TlePropagator() = default;
    virtual OrbitState propagate (
        // This will be the input to give which are different from the cartesian coordinates, they are TLE
        const Tle& tle,
        const TimeUTC& targetTime
    ) const = 0;
};