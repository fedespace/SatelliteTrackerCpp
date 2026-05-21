#include "Tle.h"
#include "TimeUTC.h"
#include "TimeUtils.h"
#include <string>
#include <cmath>
#include "../../external/sgp4/SGP4.h"
#include <algorithm>

// ===============================

// Validate the two parameters strings
void validate (Tle& tle) {
    // First line must start with char 1, and second line with char 2
    if (tle.line1[0] != '1' || tle.line2[0] != '2') {
        throw std::invalid_argument("Number of lines mismatch.");
    }
    // Validate the checksum for line1 and line2
    checksumCompute(tle.line1);
    checksumCompute(tle.line2);
}

void checksumCompute(std::string TleLine) {
    // Create dummy line, a copy of the original tle line
    std::string dummy = TleLine;
    // Deleting the spaces in the string and substitute - with 1 and + and . with 0 (no value)
    dummy.erase(std::remove(dummy.begin(), dummy.end(), ' '), dummy.end());
    std::replace(dummy.begin(), dummy.end(), '-', '1');
    std::replace(dummy.begin(), dummy.end(), '+', '0');
    std::replace(dummy.begin(), dummy.end(), '.', '0');
    // Remove alpha char from the string
    for (int i = 0; i < dummy.length(); i++) {
        if (std::isalpha(static_cast<unsigned char>(dummy[i]))) {
            dummy[i] = '0';
        }
    }
    // Iterate char by char and sum all the elements
    int sum = 0;
    for (int i = 0; i < dummy.size() - 1; i++) {
        char c = dummy[i];
        int v = c - '0';
        sum += v; 
    }
    // Compute the modulo 10 of the result
    int checksum = sum % 10;
    int lastDigit = dummy.back() - '0';

    if (checksum != lastDigit) {
        throw std::invalid_argument("The provided TLE is not valid.");
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

// Parse TLE to get param needed for sgp4init()
TleParam parseTLE(Tle tle) {
    gravconsttype whichconst = wgs72; // standard for TLE
    char opsmode = 'i'; // 'improved'
    std::string satnString = tle.line1.substr(1, 5);
    std::array<char,6> satn = {}; 
    char satnArray[6];
    satnString.copy(satnArray, 5, 2);
    std::memcpy(satn.data(), satnArray, 6);
//     std::memcpy(satrec.satn, satnString.c_str(), 5);
// satrec.satn[5] = '\0';
    double epoch = jd(tle);
    double dragInt = std::stod(tle.line1.substr(54, 5)) * 0.00001;
    int divide = std::stoi(tle.line1.substr(60, 1));
    double bstar = -dragInt / pow(10, divide); // drag coefficient
    std::string ndotSign = tle.line1.substr(33, 1) + "0.";
    std::string ndotString = ndotSign + tle.line1.substr(35, 8);
    // Converting rev to rad using 2*π and day to min using 1440.0, dividing by 2 for how equations are made
    double ndot = std::stod(ndotString) * M_PI / (1440.0 * 1440.0); 
    double nddot = 0.0;
    double ecco = std::stod(tle.line2.substr(26, 7)) * 0.0000001; // eccentricity
    double argpo = std::stod(tle.line2.substr(34, 8)) * M_PI / 180.0; // arg of per [rad]
    double inclo = std::stod(tle.line2.substr(8, 8)) * M_PI / 180.0; // inclination [rad]
    double mo = std::stod(tle.line2.substr(43, 8)) * M_PI / 180.0; // mean anomaly [rad]
    double no_kozai = std::stod(tle.line2.substr(52, 11)) * 2.0 * M_PI / 1440.0; // mean motion [rad/min]
    double nodeo = std::stod(tle.line2.substr(17, 8)) * M_PI / 180.0; // RAAN [rad]

    return {
        whichconst,
        opsmode,
        satn,
        epoch,
        bstar,
        ndot,
        nddot,
        ecco,
        argpo,
        inclo,
        mo,
        no_kozai,
        nodeo
    };
}

// Define type of orbit
// OrbitClassif typeOfOrbit(const Tle& tle, double mu) {
//     OrbitClassif type;
//     // Fetch mean motion, eccentricity and inclination from TLE elements
//     double n_revDay = stod(tle.line2.substr(52,11)); // [rev/day]
//     double e = stod(tle.line2.substr(26,7))/10e6;
//     double i = stod(tle.line2.substr(8,8));
//     // Computing semi major axis, rp and ra from n. Need to initialise satrec to get constant mu
//     double n = n_revDay*(2*M_PI)/86400; // [rad/s]
//     double a = cbrt(mu/(n*n));
//     double rp = a*(1-e);
//     double ra = a*(1+e);
//     // Decision tree starting from the semi major axis
//     if (a < 8300) {
//         type = OrbitClassif::LEO;
//     } else if (a >= 8300 && a < 42164) {
//         if (e < 0.5) {
//             type = OrbitClassif::MEO;
//         } else {
//             if (ra > 46000) {
//                 type = OrbitClassif::HEO;
//             } else {
//                 type = OrbitClassif::GTO;
//             }
//         }
//     }
//     else if (a >= 42162 && a < 42167) {
//         type = OrbitClassif::GEO;
//     }

//     return type;
// }