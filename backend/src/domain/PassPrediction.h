#pragma once
#include "./Vector3D.h"
#include "./TimeUTC.h"
#include "TimeUtils.h"
#include "./sgp4Propagator.h"
#include "./Tle.h"

// ========================

// Conversion from [lat, lon, alt] to ECEF
Vector3D r_gs(double lat, double lon, double alt);

// Conversion of satellite tle @ time to ECEF
Vector3D r_sat(Tle tle, TimeUTC t);