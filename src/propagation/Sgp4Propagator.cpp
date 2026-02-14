#include "Sgp4Propagator.h"
#include "../../external/sgp4/sgp4.h"
#include "../domain/TimeUtils.h"
#include <cstdlib>
#include <cmath>

// ===============================

// Sgp4Propagator::propagate
//    Inputs:
//      - tle [Tle]
//      - targetTime [TimeUTC]
//    Outputs:
//      - {position, velocity, targetTime} [OrbitState]

OrbitState Sgp4Propagator::propagate(
    const Tle& tle,
    const TimeUTC& targetTime
) const {

    // SGP4 satellite state block initialisation (struct originated from C/Fortran)
    elsetrec satrec{};   

    // Extra arguments definition
    double typerun = 'c'; // Type of run: catalog
    double typeinput = 'e'; // TLE input: epoch
    char opsmode = 'i'; // Mode of operation: improved
    gravconsttype whichconst = wgs72; // Most of TLEs are generated using WGS-72
    double startmfe = 0.0;
    double stopmfe = 0.0;
    double deltamin = 0.0;

    // Parsing TLE and previous defined variable, placing all in the satrec{} struct
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

    // Conversion TLE epoch [year, dayOfYear] to MJD2000. Output: [min]
    int yearTLE = std::stoi("20"+tle.line1.substr(16,2));
    double dayOfYearTLE = std::stod(tle.line1.substr(19,12));
    double minutesSinceEpochTLE = epoch2MJD2000_TLE(yearTLE, dayOfYearTLE);

    // Conversion targetTime [TimeUTC] to MJD2000. Output: [min]
    double minutesSinceEpochTarget = epoch2MJD2000(targetTime);

    // Computing deltaT between TLE and finalEpoch
    double deltaT = minutesSinceEpochTarget - minutesSinceEpochTLE;

    // Initialise position and velocity vectors [TEME], and propagate the orbit using SGP4
    double r[3], v[3]; // [km], [km/s]
    SGP4Funcs::sgp4(satrec, deltaT, r, v);

    return {
        {r[0], r[1], r[2]},
        {v[0], v[1], v[2]},
        targetTime
    };
}