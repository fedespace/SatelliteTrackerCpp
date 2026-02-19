#pragma once
#include "TimeUTC.h"
#include <stdexcept>
#include <vector>

// ===============================

// TLE epoch conversion to MJD2000 [min]
double epoch2MJD2000_TLE(int year, double dayFrac);

// targetEpoch conversion to MJD2000 [min]
double epoch2MJD2000(const TimeUTC& targetTime);

// Leap year boolean
bool leapYear(int year);

// Validation of input
void validate(const TimeUTC& time);

// MJD2000 to targetEpoch conversion
TimeUTC MJD20002epoch(double mjd_date);