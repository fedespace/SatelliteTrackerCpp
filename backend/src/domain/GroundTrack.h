#pragma once
#include "TimeUTC.h"
#include "TimeUtils.h"
 #include "../frames/FrameTransforms.h"
#include "Tle.h"

// Defining the structure of each point on the groundtrack
struct GroundTrack {
    TimeUTC time;
    double lat;
    double lon;
};

// Function to compute the GT point
std::vector<GroundTrack> gtPoints(const Tle& tle, const TimeUTC& start, const TimeUTC& end, double step);

// // Function to fetch just a single point @ target time
// GroundTrack point(const Tle& tle, const TimeUTC& target);

// Display in a table the gt data
void printTableGT(const std::vector<GroundTrack> gt);