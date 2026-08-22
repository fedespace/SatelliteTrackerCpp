#pragma once
#include "./Vector3D.h"
#include "./TimeUTC.h"
#include "TimeUtils.h"
#include "./sgp4Propagator.h"
#include "./Tle.h"
#include <algorithm>

// ========================

// Defining the structure AOS LOS and max_el
struct PassPrediction {
    int id;
    TimeUTC AOS;
    TimeUTC LOS;
    double max_el;
    TimeUTC time_maxEl;
    double sharpness;
    double duration;
};

// Entire window
struct WindowElevation {
    TimeUTC time;
    double elevation;
};

// Conversion from [lat, lon, alt] to ECEF
Vector3D r_gs(double lat, double lon, double alt);

// Conversion of satellite tle @ time to ECEF
Vector3D r_sat(Tle tle, TimeUTC t);

// Rotation from ECEF to ENU, given the two r
Vector3D r_ENU(double locationLat, double locationLon, Vector3D r1, Vector3D r2);

// Computation of elevation and azimuth for pass prediction
std::vector<double> ENU2ElAz(Vector3D r_enu);

// While cycle to compute AOS and LOS as TimeUTC objects
std::vector<PassPrediction> passTimes(Tle tle, TimeUTC tstart, TimeUTC tend, double lat, double lon, double alt, double elevationMask, double step);