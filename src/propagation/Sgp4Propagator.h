#pragma once
#include "TlePropagator.h"

// ===============================

// Defining the class Sgp4Propagator, of type TlePropagator
class Sgp4Propagator: public TlePropagator {
    public:
        OrbitState propagate (
            const Tle& tle,
            const TimeUTC& targetTime
        ) const override;
};