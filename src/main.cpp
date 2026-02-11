#include <iostream>
#include "domain/OrbitState.h" 
#include "propagation/ConstantVelocityPropagator.h" // can be deleted
#include "frames/Teme2Ecef.h"
#include "tle/TleParser.h"
#include "propagation/Sgp4Propagator.h"

int main() {
    
    try {
        // Create an OrbitState object.
        // Don't use equal sign after the variable, as it is safer to do direct-list initialization to avoid catastrophic errors
        // OrbitState sat {
        //     {7000.0, 0.0, 0.0},
        //     {0.0, 7.5 , 0.0},
        //     {2026, 1, 30, 12, 0, 0.0}
        // };

        // Validation of the time provided: if this raises the error invalid_argument, then the code goes straight to the catch section below.
        // sat.UNIXepoch.validate();

        // Let's use a target date for the constant velocity propagator, verifying it is valid.
        // TimeUTC finalEpoch{2026,2,10,0,0,0.0};
        // finalEpoch.validate();

        // Compute the norm of the state position vector using the function I have implemented
        // std::cout << "Position norm [km]: " << sat.position_km.norm() << std::endl;

        // Let's initialise the constant velocity propagator
        // ConstantVelocityPropagator propagator;
        // And propagate the state using it
        // OrbitState propagated_constantVelocity = propagator.propagate(sat, finalEpoch);
        // std::cout << "Propagation successful. Y position after 10 minutes of propagation: " << propagated_constantVelocity.position_km.y << std::endl;

        // Let's get the Eci and Ecef coordinates from the result
        // Vector3D rEci = propagated_constantVelocity.position_km;
        // double dt = secondsBetween(sat.UNIXepoch, finalEpoch);
        // double theta = EARTH_ROT_RATE_RAD_S * dt;
        // Matrix3x3 Rz = rotationMatrixZ(theta);
        // Vector3D rEcef = rotateZ(Rz, rEci);

        // Printing the results for component x of the two vectors:
        // std::cout << "Eci vector: " << rEci.x << ", Ecef vector: " << rEcef.x << std::endl; 

        // Parse now ISS TLE:
        Tle twoLineElements;
        twoLineElements.name = 
        "0 COSMOS 2517 (GEO-IK)";
        // twoLineElements.line1 = 
        // "1 21835U 86033A   26036.12345678  .00000012  00000-0  12345-4 0  9991";
        // twoLineElements.line2 = 
        // "2 21835   0.0123 123.4567 0001234 123.4567 123.4567  1.00234567 12345";
        twoLineElements.line1 = "1 25544U 98067A 26042.12345678 .00001234 00000-0 12345-4 0 9999";
        twoLineElements.line2 = "2 25544 51.6420 123.4567 0001234 123.4567 123.4567 15.72123456 12345";
        TimeUTC target{2026, 2, 11, 2, 57, 11.0};
        twoLineElements.validate();
        twoLineElements.standardise();
        target.validate();

        // Target time
        // TimeUTC target{2026,2,15,0,0,0.0};
        target.validate();

        // After trying to propagate a random r vector using a constant velocity propagator, let's try now to propagate a TLE. That means parsing it and propagate using SGP4 propagator and then convert the TEME resulting vectors to ECEF by rotating it around the Z-axis
        Sgp4Propagator SGP4_TLE;
        OrbitState rv = SGP4_TLE.propagate(twoLineElements, target);

        std::cout << "Position vector norm after propagation: " << rv.position_km.norm() << std::endl;

        // Checking the values of position vector
        std::cout << "With components: " << rv.position_km.x <<", "<< rv.position_km.y <<", "<< rv.position_km.z << std::endl;

        // double finalEpochMins = epoch2MJD2000(finalEpoch);
        // double TLEMins = epoch2MJD2000_TLE(std::stoi(ISS.line1.substr(19, 2)),std::stod(ISS.line1.substr(21, 12)));
        // double dM = finalEpochMins - TLEMins;
        // std::cout << "time delta from MJD2000 to final epoch [min]: " << finalEpochMins;
        // std::cout << "time delta from MJD2000 to TLE epoch [min]: " << TLEMins;
        // std::cout << "delta t: " << dM;

        // // Conversion using rotation angle from a reference epoch
        // //TimeUTC mjd2000{2000,1,1,0,0,0.0};
        // Vector3D rECEF = TEME2ECEF(rv.position_km, finalEpoch);
        // std::cout << "position vector in ECEF: " << rECEF.norm();
        // std::cout << "with components: " << rECEF.x << rECEF.y << rECEF.z;

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what();
        return 1;
    }
    return 0;
}