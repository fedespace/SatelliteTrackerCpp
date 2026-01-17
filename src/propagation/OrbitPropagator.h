#pragma once
#include "../domain/OrbitState.h"
#include "../domain/TimeUTC.h"

// ===============================

class OrbitPropagator {     // class because it is going to detail a behaviour, not just data
    public:
        // will firstly build the destructor
        virtual ~OrbitPropagator() = default;

        // this will be the function we'll call no matter which kind of prop we are using. Inside the parenthesis we have the two arguments which are initial state and ending epoch.
        virtual OrbitState propagate(
            const OrbitState& initialState,
            const TimeUTC& finalEpoch
        ) const = 0;
};