#pragma once
#include <string>

// ===============================

struct Tle {
    std::string name;
    std::string line1;
    std::string line2;
};

void validate(Tle& tle);

void standardise (Tle& tle);

std::string typeOfOrbit(const Tle& tle, double mu);