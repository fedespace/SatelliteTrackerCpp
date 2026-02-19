#include "Tle.h"
#include "TimeUTC.h"
#include "OrbitState.h"
#include <string>
#include <cmath>
#include "../../external/sgp4/sgp4.h"

// ===============================

// Validate the two parameters strings
void validate (Tle& tle) {
    if (tle.line1[0] != '1' || tle.line2[0] != '2') {
        throw std::invalid_argument("Number of lines mismatch.");
    }
}

// Clean spaces and special characters to make it standard format
void standardise (Tle& tle) {
    for (char& c : tle.line1)
        if (c == '\t') c = ' ';
    for (char& c : tle.line2)
        if (c == '\t') c = ' ';
    if (tle.line1.size() < 69)
        tle.line1.append(69 - tle.line1.size(), ' ');
    if (tle.line2.size() < 69)
        tle.line2.append(69 - tle.line2.size(), ' ');
}

// Define type of orbit
OrbitClassif typeOfOrbit(const Tle& tle, double mu) {
    OrbitClassif type;
    // Fetch mean motion, eccentricity and inclination from TLE elements
    double n_revDay = stod(tle.line2.substr(52,11)); // [rev/day]
    double e = stod(tle.line2.substr(26,7))/10e6;
    double i = stod(tle.line2.substr(8,8));
    // Computing semi major axis, rp and ra from n. Need to initialise satrec to get constant mu
    double n = n_revDay*(2*M_PI)/86400; // [rad/s]
    double a = cbrt(mu/(n*n));
    double rp = a*(1-e);
    double ra = a*(1+e);
    // Decision tree starting from the semi major axis
    if (a < 8300) {
        type = OrbitClassif::LEO;
    } else if (a >= 8300 && a < 42164) {
        if (e < 0.5) {
            type = OrbitClassif::MEO;
        } else {
            if (ra > 46000) {
                type = OrbitClassif::HEO;
            } else {
                type = OrbitClassif::GTO;
            }
        }
    }
    else if (a >= 42162 && a < 42167) {
        type = OrbitClassif::GEO;
    }

    return type;
}