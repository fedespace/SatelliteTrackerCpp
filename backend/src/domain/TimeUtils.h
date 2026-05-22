#pragma once
#include "TimeUTC.h"
#include <stdexcept>
#include <vector>
#include "Tle.h"

// ===============================

// JD of TLE epoch [days, double]
double jd(Tle tle);

// TLE epoch conversion to MJD2000 [min]
double epoch2MJD2000_TLE(Tle tle);

// targetEpoch conversion to MJD2000 [min]
double epoch2mins(const TimeUTC& targetTime);

// Leap year boolean
bool leapYear(int year);

// Validation of input
void validate(const TimeUTC& time);

// MJD2000 to targetEpoch conversion
TimeUTC MJD20002epoch(double mjd_date);

// Conversion from TimeUTC to String
std::string to_iso8601(TimeUTC time);