#pragma once
#include "OrbitPropagator.h"
#include "../domain/TimeUtils.h"

// ===============================

class ConstantVelocityPropagator : public OrbitPropagator
{
public:
    OrbitState propagate(
        const OrbitState &initialState,
        const TimeUTC &finalEpoch) const override
    {
        // let's compute the difference between the two instants.
        // ASSUMPTION: t0 and t1 are both in the same day.
        double dt = secondsBetween(initialState.UNIXepoch, finalEpoch);

        // Consider the result and then update both UNIX and position_km
        OrbitState result;
        result.UNIXepoch = finalEpoch;

        // Considering the equation r(t) = r(0) + v0*Δt, where r(0) is the initialState I previously defined:
        result.position_km = {
            initialState.position_km.x + initialState.velocity_kms.x*dt,
            initialState.position_km.y + initialState.velocity_kms.y*dt,
            initialState.position_km.z + initialState.velocity_kms.z*dt
        };
        result.velocity_kms = initialState.velocity_kms;

        return result;
    }
};