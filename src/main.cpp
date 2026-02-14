#include <iostream>
#include "domain/OrbitState.h"
#include "frames/Teme2Ecef.h"
#include "propagation/Sgp4Propagator.h"

int main() {
    
    try {
        // Parse provided TLE: initialise the object and the attributes
        // Then validate the TLE and standardise the format
        Tle twoLineElements;
        twoLineElements.name = "0 COSMOS 2517 (GEO-IK)";
        twoLineElements.line1 = "1 25544U 98067A 26042.12345678 .00001234 00000-0 12345-4 0 9999";
        twoLineElements.line2 = "2 25544 51.6420 123.4567 0001234 123.4567 123.4567 15.72123456 12345";
        twoLineElements.validate();
        twoLineElements.standardise();

        // Iitialise the target epoch and validate it
        TimeUTC target{2026, 4, 30, 2, 57, 11.0};
        validate(target);

        // Create the object SGP4_TLE of class SGP4Propagator
        // Propagate the provided TLE using the funcion of the propagator
        Sgp4Propagator SGP4_TLE;
        OrbitState rv = SGP4_TLE.propagate(twoLineElements, target);
        std::cout << "Position vector norm after propagation: " << rv.position_km.norm() << ", x = " << rv.position_km.x <<", y = "<< rv.position_km.y <<", z = "<< rv.position_km.z << std::endl;

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