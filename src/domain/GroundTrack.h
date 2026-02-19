#pragma once
#include "TimeUTC.h"
#include "Tle.h"

// Defining the structure of each point on the groundtrack
struct GroundTrack {
    TimeUTC time;
    double lat;
    double lon;
};

// Function to compute the GT point
GroundTrack gtPoints(const Tle& tle, const TimeUTC& start, const TimeUTC& end, double step);