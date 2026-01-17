#include <iostream>
#include "domain/OrbitState.h" // only this is enough, it contains the others
#include "propagation/ConstantVelocityPropagator.h"

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

        // trying to use a target date for the constant velocity propagator, and verify that
        TimeUTC finalEpoch{2026,1,16,12,10,0.0};
        finalEpoch.validate();

        std::cout << "Position norm [km]: " << sat.position_km.norm() << std::endl;

        // Let's initialise the constant velocity propagator
        ConstantVelocityPropagator propagator;
        // And let's propagate the state using it
        OrbitState propagated = propagator.propagate(sat, finalEpoch);

        std::cout << "Propagation successful. New epoch minute is: " << propagated.UNIXepoch.minute << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what();
        return 1;
    }
    return 0;
}