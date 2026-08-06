#include <iostream>
#include <iomanip>
#include "domain/sgp4Propagator.h"
#include "./domain/PassPrediction.h"

// ===============================

int main() {
    
    // Get ECEF from ground station coordinates
    double lat = 51.42;
    double lon = -2.85;
    double alt = 15.0;
    double elevationMask = 20.0;
    double step = 5.0; // [sec]

    Tle iss;
    iss.name = "ISS (ZARYA)";   
    iss.line1 = "1 25544U 98067A   26215.79638706  .00007444  00000+0  14146-3 0  9999";
    iss.line2 = "2 25544  51.6316  64.4821 0007224   9.2337 350.8783 15.49332738579132";

    TimeUTC t0 = {2026, 8, 5, 14, 45, 0.0};
    TimeUTC tend = {2026, 8, 5, 15, 5, 0.0};

    std::vector<PassPrediction> contacts = passTimes(iss, t0, tend, lat, lon, alt, elevationMask, step);

    for (int i = 0; i < contacts.size(); i++) {
        std::cout << time2string(contacts[i].AOS) << "\n"; // << " " << time2string(contacts[i].LOS) << " " << contacts[i].max_el << "\n" ;
    }
    

    return 0;
}
