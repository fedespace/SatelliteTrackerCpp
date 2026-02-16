#include "Sgp4Propagator.h"
#include "../../external/sgp4/sgp4.h"
#include "../domain/TimeUtils.h"
#include <cstdlib>
#include <cmath>
#include "../domain/Tle.h"

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
    int yearTLE = std::stoi("20"+tle.line1.substr(18,2));
    double dayOfYearTLE = std::stod(tle.line1.substr(20,12));
    double minutesSinceEpochTLE = epoch2MJD2000_TLE(yearTLE, dayOfYearTLE);

    // Conversion targetTime [TimeUTC] to MJD2000. Output: [min]
    double minutesSinceEpochTarget = epoch2MJD2000(targetTime);

    // Given the type or orbit, check maxDT, idealDT and errorMax
    double mu = satrec.mus;
    std::string type = typeOfOrbit(tle, mu);
    int maxDT;
    int idealDT;
    int errorMax;
    if (type == "LEO") {
        idealDT = 1440; // 1 day
        maxDT = 1440 * 3; // 3 days
        errorMax = 10; // [km]
    } else if (type == "MEO") {
        maxDT = 1440 * 3; // 3 days
        idealDT = maxDT;
        errorMax = 20;
    } else if (type == "GTO" || type == "HEO") {
        maxDT = 1440 * 2; // 2 days
        idealDT = maxDT;
        errorMax = 0; // info unavailable
    } else if (type == "GEO") {
        maxDT = 1440 * 15; // 2 weeks
        idealDT = maxDT;
        errorMax = 0; // info unavailable
    }

    std::cout << "Type of orbit: " << type << std::endl;

    // Computing deltaT between TLE and finalEpoch
    double deltaT = minutesSinceEpochTarget - minutesSinceEpochTLE;
    // std::cout << "Min tle: " << minutesSinceEpochTLE << " and min target: " << minutesSinceEpochTarget << std::endl;
    // std::cout << deltaT << std::endl;

    // Initialising vectors 
    double r[3], v[3]; // [km], [km/s]

    // Compare to deltaT obtained in the previous step
    if (deltaT > maxDT) {
        std::cout << "TLE is invalid for this target epoch.\n";
        std::cout << "Aborting...\n";
    } else if (deltaT >= idealDT && deltaT <= maxDT) {
        std::cout << "TLE in the range of acceptable time frame.\n";
        if (errorMax != 0) {
            std::cout << "Maximum expected error: " << errorMax << " [km].\n";
        }
        std::cout << "Proceeding with the computation...\n";
        // Running SGP4
        SGP4Funcs::sgp4(satrec, deltaT, r, v);
    } else {
        std::cout << "TLE is optimally valid for this specific target epoch.\n";
        std::cout << "Proceeding with the computation...\n";
        SGP4Funcs::sgp4(satrec, deltaT, r, v);
    }

    return {
        {r[0], r[1], r[2]},
        {v[0], v[1], v[2]},
        targetTime
    };
}

JD Sgp4Propagator::jday_SGP4(
    const TimeUTC targetTime
) {
    // Initialise the JD and JDfrac
    double jd;
    double jdFrac;

    // Compute
    SGP4Funcs::jday_SGP4(targetTime.year, targetTime.month, targetTime.day, targetTime.hour, targetTime.minute, targetTime.second, jd, jdFrac);

    return {
        jd, 
        jdFrac
    };
}

double Sgp4Propagator::gstime_SGP4(
    JD julianDay
) {
    double gmst = SGP4Funcs::gstime_SGP4(julianDay.jd);
    return gmst;
}

