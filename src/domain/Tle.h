#pragma once
#include <string>

// ===============================

struct Tle {
    std::string name;
    std::string line1;
    std::string line2;


    void validate () {
        if (line1[0] != '1' || line2[0] != '2') {
            throw std::invalid_argument("Number of lines mismatch.");
        }
    }

    void standardise () {
        for (char& c : line1)
            if (c == '\t') c = ' ';
        for (char& c : line2)
            if (c == '\t') c = ' ';
        if (line1.size() < 69)
            line1.append(69 - line1.size(), ' ');
        if (line2.size() < 69)
            line2.append(69 - line2.size(), ' ');
    }
};