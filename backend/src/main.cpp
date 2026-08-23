#include <iostream>
#include <iomanip>
#include "./domain/sgp4Propagator.h"
#include "./domain/PassPrediction.h"

// ===============================

int main() {
    
    // Get ECEF from ground station coordinates
    double lat = 52;
    double lon = 0;
    double alt = 30.0;
    double elevationMask = 10.0;
    double step = 5.0; // [sec]

    Tle iss;
    iss.name = "ISS (ZARYA)";   
    iss.line1 = "1 25544U 98067A   26234.50053383  .00009133  00000+0  17025-3 0  9997";
    iss.line2 = "2 25544  51.6331 331.8814 0007668  72.6488 287.5339 15.49570248582031";

    TimeUTC t0 = {2026, 8, 24, 0, 0, 0.0};
    TimeUTC tend = {2026, 8, 24, 23, 59, 59.0};

    std::vector<PassPrediction> contacts = passTimes(iss, t0, tend, lat, lon, alt, elevationMask, step);

    for (int i = 0; i < contacts.size(); i++) {
        std::cout << "AOS: " << time2string(contacts[i].AOS) << ", LOS: " << time2string(contacts[i].LOS) << ", duration: " << contacts[i].duration << "\n"; 
    }
    

    return 0;
}
