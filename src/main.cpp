#include <iostream>
#include "domain/OrbitState.h" 
#include "propagation/ConstantVelocityPropagator.h"
#include "frames/EciToEcef.h"
#include "tle/TleParser.h"

int main() {
    
    try {
        // Create an OrbitState object.
        // Don't use equal sign after the variable, as it is safer to do direct-list initialization to avoid catastrophic errors
        OrbitState sat {
            {7000.0, 0.0, 0.0},
            {0.0, 7.5 , 0.0},
            {2026, 1, 16, 12, 0, 0.0}
        };

        // Validation of the time provided: if this raises the error invalid_argument, then the code goes straight to the catch section below.
        sat.UNIXepoch.validate();

        // Let's use a target date for the constant velocity propagator, verifying it is valid.
        TimeUTC finalEpoch{2026,1,16,12,10,0.0};
        finalEpoch.validate();

        // Compute the norm of the state position vector using the function I have implemented
        std::cout << "Position norm [km]: " << sat.position_km.norm() << std::endl;

        // Let's initialise the constant velocity propagator
        ConstantVelocityPropagator propagator;
        // And propagate the state using it
        OrbitState propagated_constantVelocity = propagator.propagate(sat, finalEpoch);

        std::cout << "Propagation successful. Y position after 10 minutes of propagation: " << propagated_constantVelocity.position_km.y << std::endl;

        // Let's get the Eci and Ecef coordinates from the result
        Vector3D rEci = propagated_constantVelocity.position_km;
        double dt = secondsBetween(sat.UNIXepoch, finalEpoch);
        double theta = EARTH_ROT_RATE_RAD_S * dt;
        Matrix3x3 Rz = rotationMatrixZ(theta);
        Vector3D rEcef = rotateZ(Rz, rEci);

        // Printing the results for component x of the two vectors:
        std::cout << "Eci vector: " << rEci.x << ", Ecef vector: " << rEcef.x << std::endl; 

        // Parse now ISS TLE:
        Tle ISS = TleParser (
            "ISS (ZARYA)",
            "1 25544U 98067A   26020.17509289  .00021194  00000+0  38548-3 0  9998",
            "2 25544  51.6334 312.1983 0007785  38.3265 321.8276 15.49442598548811"
        );

        std::cout << ISS.name;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what();
        return 1;
    }
    return 0;
}