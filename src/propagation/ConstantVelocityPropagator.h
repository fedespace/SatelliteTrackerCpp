#pragma once
#include "OrbitPropagator.h"

// ===============================

class ConstantVelocityPropagator: public OrbitPropagator {
    public:
    OrbitState propagate(
        const OrbitState& initialState,
        const TimeUTC& finalEpoch
    ) const override {
        // we are returning the same state but with a different epoch
        OrbitState result = initialState;
        result.UNIXepoch = finalEpoch;
        return result;
    }
};