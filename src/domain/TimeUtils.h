#pragma once
#include "TimeUTC.h"

// ===============================

// will build an inline function so that we eliminate the (small) overhead by calculating everything in a single step.
// we are assuming we are dealing with times in the same day
inline double secondsBetween(const TimeUTC& t0, const TimeUTC& t1) {
    return (t1.hour - t0.hour) * 3600.0 +
            (t1.minute - t0.minute) * 60.0 +
            (t1.second - t0.second);
}