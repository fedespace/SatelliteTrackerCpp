#include <iostream>
#include "domain/OrbitState.h" // only this is enough, it contains the others

int main() {
    
    try {
        // Create an OrbitState object.
        // Compute the norm of the position vector and print it.
        // Don't use equal sign after the variable, as it is safer to do direct-list initialization to avoid catastrophic errors
        OrbitState sat {
            {6597.0, 7152.0, 0.0},
            {0.0, 7.5 , 1.0},
            {2026, 1, 16, 12, 0, 0.0}
        };

        sat.UNIXepoch.validate(); // If this raises the error invalid_argument, then the code goes straight to the catch section below.

        std::cout << "Position norm [km]: " << sat.position_km.norm();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what();
        return 1;
    }
    return 0;
}