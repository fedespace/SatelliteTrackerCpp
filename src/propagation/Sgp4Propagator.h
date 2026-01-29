#pragma once
#include "TlePropagator.h"

// ===============================

// Saying that this class we are defining "Sgp4Propagator" is of type "TlePropagator" previously defined.
class Sgp4Propagator: public TlePropagator {
    public:
        // what is going to result from the initialization, is an object of type "OrbitState"
        OrbitState propagate (
            // inputs are:
            const Tle& tle,
            const TimeUTC& targetTime
        ) const override;
};