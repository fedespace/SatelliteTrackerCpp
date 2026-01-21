#pragma once
#include "../domain/Tle.h"
#include <stdexcept>
#include <iostream>

// ===============================

inline Tle TleParser (
    const std::string& name,
    const std::string& line1,
    const std::string& line2
) {
    // Throw the exception in case the validation fails:
    if (line1.length() < 69 || line2.length() < 69) {
        throw std::invalid_argument("TLE lines must be of size greater than 69.");
    }
    if (line1[0] != '1' || line2[0] != '2') {
        throw std::invalid_argument("Number of lines mismatch.");
    }

    return {name, line1, line2};
}