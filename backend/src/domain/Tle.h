#pragma once
#include <string>
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <array>
#include "../../external/sgp4/SGP4.h"
#include <tuple>

// ===============================

struct Tle {
    std::string name;
    std::string line1;
    std::string line2;
};

struct TleParam {
    gravconsttype whichconst;
    char opsmode; 
    std::array<char,6> satn;
    double epoch; // from 31st Dec 1950 [days]
    double bstar; 
    double ndot;
    double nddot;
    double ecco; 
    double argpo; // [rad]
    double inclo; // [rad]
    double mo; // [rad]
    double no_kozai; // [rad/min]
    double nodeo; // [rad]
};

void validate(Tle& tle);

void checksumCompute(std::string TleLine);

void standardise (Tle& tle);

TleParam parseTLE(Tle tle);

std::string orbit_type(Tle tle);

std::tuple<int, std::string> max_delta(std::string classification_orbit);