#include <iostream>
#include "domain/OrbitState.h"
#include "frames/Teme2Ecef.h"
#include "propagation/Sgp4Propagator.h"

int main() {
    
    try {
        // Parse provided TLE: initialise the object and the attributes
        // Then validate the TLE and standardise the format
        Tle twoLineElements;
        // LEO EXAMPLE - ONEWEB
        // twoLineElements.name = "ONEWEB-0162";
        // twoLineElements.line1 = "1 48056U 21025Q   26046.24572298 -.00000136  00000+0 -40788-3 0  9991";
        // twoLineElements.line2 = "2 48056  87.9044 344.4620 0001950  75.0816 285.0529 13.13470762237537";

        // MEO EXAMPLE - GALILEO
        // twoLineElements.name = "GSAT0204 (GALILEO 8)";
        // twoLineElements.line1 = "1 40545U 15017B   26045.28570554 -.00000086  00000+0  00000+0 0  9998";
        // twoLineElements.line2 = "2 40545  56.8343 344.9670 0004749 284.0178  75.9292  1.70475398 30153";

        // GEO EXAMPLE - EUTELSAT
        twoLineElements.name = "EUTELSAT 36B";
        twoLineElements.line1 = "1 36101U 09065A   26046.26512253  .00000117  00000+0  00000+0 0  9992";
        twoLineElements.line2 = "2 36101   0.1813  75.9547 0005694 253.1744 322.0673  1.00270201 59485";
        validate(twoLineElements);
        standardise(twoLineElements);
        std::cout << "\nTLE selected: \n";
        std::cout << twoLineElements.line1 << std::endl;
        std::cout << twoLineElements.line2 << std::endl;

        // Iitialise the target epoch and validate it
        // TimeUTC target{2026, 2, 16, 0, 0, 0.0}; // for Oneweb LEO case
        TimeUTC target{2026, 2, 25, 0, 0, 0.0}; // for Oneweb LEO case
        validate(target);
        std::string targetString = std::to_string(target.day) + "/" + std::to_string(target.month) + "/" + std::to_string(target.year) + ", " + std::to_string(target.hour) + ":" + std::to_string(target.minute) + ":" + std::to_string(target.second) + "\n"; 
        std::cout << "\nTarget time selected: " << targetString << std::endl;

        // Create the object SGP4_TLE of class SGP4Propagator
        // Propagate the provided TLE using the funcion of the propagator
        Sgp4Propagator SGP4_TLE;
        OrbitState rv = SGP4_TLE.propagate(twoLineElements, target);
        std::cout << "Position vector norm after propagation: " << rv.position_km.norm() << " [km], component x = " << rv.position_km.x <<", y = "<< rv.position_km.y <<", z = "<< rv.position_km.z << std::endl;

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