#include <iostream>
#include "domain/OrbitState.h"
#include "frames/Teme2Ecef.h"
#include "propagation/Sgp4Propagator.h"
#include "frames/FrameTransforms.h"

// ===============================

int main() {
    
    try {
        Tle twoLineElements;
        
        // Validation: example of TLE for LEO, MEO and GEO for validating the consistency of results
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
        
        std::cout << "\nTLE selected: " << twoLineElements.name << std::endl;
        std::cout << twoLineElements.line1 << std::endl;
        std::cout << twoLineElements.line2 << std::endl;

        // Iitialise the target epoch and validate it
        TimeUTC target{2026, 2, 16, 0, 0, 0.0}; // for Oneweb LEO case
        
        validate(target);

        std::string targetString = std::to_string(target.day) + "/" + std::to_string(target.month) + "/" + std::to_string(target.year) + ", " + std::to_string(target.hour) + ":" + std::to_string(target.minute) + ":" + std::to_string(target.second) + "\n"; 
        std::cout << "\nTarget time selected: " << targetString << std::endl;

        // Create the object SGP4_TLE of class SGP4Propagator
        Sgp4Propagator SGP4_TLE;

        // Propagate the provided TLE using the funcion of the propagator
        OrbitState rv = SGP4_TLE.propagate(twoLineElements, target);
        
        std::cout << "Position vector norm after propagation (TEME): " << rv.position_km.norm() << " [km], component x = " << rv.position_km.x <<", y = "<< rv.position_km.y <<", z = "<< rv.position_km.z << std::endl;

        // targetTime to JD using Vallado's library
        JD targetJD = SGP4_TLE.jday_SGP4(target);
        // std::cout << "JD: " << targetJD.jd << std::endl;

        // Apply gstime function from Vallado's library to get GMST, used to rotate rv
        double gmst = SGP4_TLE.gstime_SGP4(targetJD); // [rad]
        // std::cout << "GMST: " << gmst << std::endl;

        // Rotation matrix from TEME to ECEF 
        Matrix3x3 rotationMatrix = rotationMatrixZ(gmst);

        // Application of the rotation to the vector rv (TEME)
        Vector3D r_ECEF = rotateZ(rotationMatrix, rv.position_km); 

        std::cout << "rECEF: {" << r_ECEF.x << ", " << r_ECEF.y << ", " << r_ECEF.z << "}\n";

        // Other rECEF vectors for validation: 
        // r_ECEF = {6378.137, 0.0, 0.0}; // expected [lat 0, lon 0]
        // r_ECEF = {3194.419, 3194.419, 4487.348}; // expected [lat 45, lon -45]
        // r_ECEF = {961.996, -555.386, 6260.372}; // expected [lat 80, lon -30]

        // Converting r_ECEF to [Lat, Long] pair
        LL ll = ecef2ll(r_ECEF);
        double lat = ll.lat;
        double lon = ll.lon;
        std::cout << "Lon, lat [deg]: {" << lon << ", " << lat << "}\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what();
        return 1;
    }
    return 0;
}