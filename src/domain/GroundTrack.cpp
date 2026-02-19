#pragma once
#include "Tle.h"
#include "TimeUTC.h"
#include "GroundTrack.h"
#include <vector>

// From the h
// GroundTrack gtPoints(const Tle& tle, const TimeUTC& start, const TimeUTC& end, double step);

GroundTrack gtPoints(const Tle& tle, const TimeUTC& start, const TimeUTC& end, double step) {
    // Initialise the points vector where data will be stored after each iteration
    std::vector<GroundTrack> points;
    
    // Conversion of start and end time in jd minutes

}