#pragma once
#include "TimeUTC.h"
#include <stdexcept>

// ===============================

// Assuming we are dealing with times in the same day
inline double secondsBetween(const TimeUTC& t0, const TimeUTC& t1) {
    return (t1.hour - t0.hour) * 3600.0 +
            (t1.minute - t0.minute) * 60.0 +
            (t1.second - t0.second);
}



// Minutes from epoch to MJD2000 (decimal point)
// TLE will come as (year, dayOfYear)
// targetEpoch will come as (year, month, day, hour, minute, second)
double epoch2MJD2000(const TimeUTC& targetTime);
double epoch2MJD2000_TLE(int year, double dayFrac);

// Computing the leap year
bool leapYear(int year);