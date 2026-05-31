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

// Compute the type of orbit
std::string orbit_type(Tle tle) {
    // Get parameters from TLE:
    double no_kozai = std::stod(tle.line2.substr(52, 11)) * 2.0 * M_PI / 86400.0; // [rad/sec]
    double earth_grav_constant = 3.98600441e14;
    double a = std::pow(earth_grav_constant / (no_kozai*no_kozai), 1.0/3.0) / 1000.0; // [m]
    double ecco = std::stod(tle.line2.substr(26, 7)) * 0.0000001; 
    double inclo = std::stod(tle.line2.substr(8, 8)); // [deg]
    double radius_apogee = a * (1 - ecco); // [km]
    // IF statement
    std::string type;
    if (a < 8400) {
        type = "LEO";
    } else if (a > 8400 && a < 42164) {
        if (ecco < 0.4) {
            type = "MEO";
        } else {
            if (radius_apogee >= 40000) {
                type = "HEO";
            } else {
                type = "GTO";
            }
        }
    } else {
        type = "GEO";
    }
    return type;
}

std::tuple<int, std::string> max_delta(std::string classification_orbit) {
    int maxDT;
    std::string degrad_degree;
    if (classification_orbit == "LEO") {
        maxDT = 7 * 86400; // 7 days
        degrad_degree = "1 to 3 km per day";
    } else if (classification_orbit == "MEO") {
        maxDT = 7 * 86400 * 3; // 3 weeks
        degrad_degree = "0.5 to 1.5 km per day, up to 5 to 10 km per week";
    } else if (classification_orbit == "GEO") {
        maxDT = 7 * 86400 * 4; // 4 weeks
        degrad_degree = "1 to 2 km per day";
    } else if (classification_orbit == "GTO") {
        maxDT = 3 * 86400;
        degrad_degree = "5 to 20+ km per day, up to 10 of hundreds of km after 3 days";
    } else if (classification_orbit == "HEO") {
        degrad_degree = "5 to 20+ km per day, up to 10 of hundreds of km after 3 days";
    }
    return {maxDT, degrad_degree};
}