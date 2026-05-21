#pragma once
#include "../../external/sgp4/SGP4.h"
#include "TimeUTC.h"
#include "Tle.h"
#include "TimeUtils.h"
#include "../frames/FrameTransforms.h"

// ========================

// Defining the structure of each point on the groundtrack
struct GroundTrack {
    TimeUTC time;
    double lat;
    double lon;
};

std::vector<GroundTrack> propagate (Tle tle, TimeUTC start, TimeUTC end, double step);