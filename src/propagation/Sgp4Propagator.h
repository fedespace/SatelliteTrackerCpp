#pragma once
#include "TlePropagator.h"
#include "../domain/JD.h"
#include "../../external/sgp4/sgp4.h"

// ===============================

// Defining the class Sgp4Propagator, of type TlePropagator
class Sgp4Propagator: public TlePropagator {
    public:
        OrbitState propagate (
            const Tle& tle,
            const TimeUTC& targetTime
        ) const override;

        JD jday_SGP4 (
            const TimeUTC targetTime
        );

        double gstime_SGP4 (
            const JD julianDay
        );
};