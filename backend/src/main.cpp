#include <iostream>
#include <iomanip>
#include "domain/sgp4Propagator.h"
#include "./domain/PassPrediction.h"

// ===============================

int main() {
    
    // Get ECEF from ground station coordinates
    double lat = 52.0;
    double lon = 0.0;
    double alt = 15;

    Vector3D gs_ecef = r_gs(lat, lon, alt);
    std::cout << "GS components ECEF: " << std::setprecision(10) << "[" << gs_ecef.x << ", " << gs_ecef.y << ", " << gs_ecef.z << "]\n"; 


    // Get ECEF from TLE (using sgp4 prop)
    Tle iss;
    iss.name = "ISS (ZARYA)";        
    iss.line1 = "1 25544U 98067A   26212.11974625  .00008690  00000+0  16406-3 0  9990";
    iss.line2 = "2 25544  51.6317  82.6819 0007117 356.3115   3.7820 15.49277094578568";
    TimeUTC t0 = {2026, 7, 31, 9, 30, 0.0};
    
    Vector3D sat_ecef = r_sat(iss, t0); 
    std::cout << "Satellite components ECEF: " << std::setprecision(10) << "[" << sat_ecef.x << ", " << sat_ecef.y << ", " << sat_ecef.z << "]\n";

    // Compute difference of the two vectors (gs vs tle @ time) -> r_sat - r_gs for the observation point
    Vector3D range;
    range.x = sat_ecef.x - gs_ecef.x;
    range.y = sat_ecef.y - gs_ecef.y;
    range.z = sat_ecef.z - gs_ecef.z;

    std::cout << "Range vector components: " << std::setprecision(10) << "[" << range.x << ", " << range.y << ", " << range.z << "], with NORM: " << range.norm() <<"\n";

    // Next step: compute range vector in ENU

    return 0;
}
