#include "Sgp4Propagator.h"
#include "../../external/sgp4/sgp4.h"
#include "../domain/TimeUtils.h"
#include <cstdlib>
#include <cmath>

// ===============================

// from this function we'll get a orbit state object with position and velocity vector plus time, using the function propagate() of the object SGP4Propagator

OrbitState Sgp4Propagator::propagate(
    const Tle& tle,
    const TimeUTC& targetTime
) const {
    // using SGP4 library internal satellite record
    elsetrec satrec{};   

    // defining extra arguments:
    double typerun = 'c'; // catalog run
    double typeinput = 'e'; // input TLE
    char opsmode = 'i'; // AFSPC Mode, check what it is
    gravconsttype whichconst = wgs72;
    // depending on your repo
    double startmfe = 0.0;
    double stopmfe = 0.0;
    double deltamin = 0.0;


    // parse TLE lines into satrec: convert the char string to sgp4 elements
    // now everything will be under a single variable called "satrec" out of which we can get the specific parameters (i.e. satrec.operationmode etc.)
    SGP4Funcs::twoline2rv(
        const_cast<char*>(tle.line1.c_str()),
        const_cast<char*>(tle.line2.c_str()),
        typerun,
        typeinput,
        opsmode,
        whichconst,
        startmfe,
        stopmfe,
        deltamin,
        satrec
    );

    // Computing mins since MJD2000 epoch from TLE epoch
    int yearTLE = std::stoi("20"+tle.line1.substr(16,2));
    double dayOfYearTLE = std::stod(tle.line1.substr(19,12));

    double minutesSinceEpochTLE = epoch2MJD2000_TLE(yearTLE, dayOfYearTLE);

    // Computing mins since MJD2000 epoch from targetTime
    double minutesSinceEpochTarget = epoch2MJD2000(targetTime);

    // Difference target time and tle epoch time
    double tsince = minutesSinceEpochTarget - minutesSinceEpochTLE;

    std::cout << "tsince variable [min]: " << tsince << std::endl;

    // Propagate orbit using SGP4, initialising position and velocity vectors
    double r[3], v[3]; // TEME reference frame in [km] and [km/s]
    SGP4Funcs::sgp4(satrec, tsince, r, v);
        
    // The return r and v vector will be in TEME (True Equator, Mean Equinox) frame, which is quasi-inertial Earth-centered frame so the Earth doesn't rotate beneath the satellite. They will need to be converted into ECEF frame and then geodetic coordinates to get latitude and longitude
    return {
        {r[0], r[1], r[2]},
        {v[0], v[1], v[2]},
        targetTime
    };
}