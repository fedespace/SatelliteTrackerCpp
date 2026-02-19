#pragma once
#include <string>
#include "OrbitState.h"

// ===============================

struct Tle {
    std::string name;
    std::string line1;
    std::string line2;
};

void validate(Tle& tle);

void standardise (Tle& tle);

OrbitClassif typeOfOrbit(const Tle& tle, double mu);