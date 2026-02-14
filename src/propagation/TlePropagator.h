#pragma once
#include "../domain/Tle.h"
#include "../domain/OrbitState.h"
#include "../domain/TimeUTC.h"

// ===============================

class TlePropagator {
public:
    virtual OrbitState propagate (
        const Tle& tle,
        const TimeUTC& targetTime
    ) const = 0;
};