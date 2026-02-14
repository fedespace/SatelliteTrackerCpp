#pragma once
#include "TimeUTC.h"
#include <stdexcept>

// ===============================

// TLE epoch conversion to MJD2000 [min]
double epoch2MJD2000_TLE(int year, double dayFrac);

// targetEpoch conversion to MJD2000 [min]
double epoch2MJD2000(const TimeUTC& targetTime);
double epoch2MJD2000_TLE(int year, double dayFrac);

// Leap year boolean
bool leapYear(int year);

// Validation of input
void validate(const TimeUTC& time);

// To be implemented: GO/NO-GO depending on orbit type (LEO, GEO, etc) and deltaT