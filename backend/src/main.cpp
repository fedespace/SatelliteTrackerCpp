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

    Tle iss;
    iss.name = "ISS (ZARYA)";        
    iss.line1 = "1 25544U 98067A   26212.11974625  .00008690  00000+0  16406-3 0  9990";
    iss.line2 = "2 25544  51.6317  82.6819 0007117 356.3115   3.7820 15.49277094578568";

    TimeUTC t0 = {2026, 7, 31, 9, 30, 0.0};
    TimeUTC tend = {2026, 8, 1, 9, 30, 0.0};

    double elevationMask = 10.0;
    double step = 5.0;

    std::vector<PassPrediction> contacts = passTimes(iss, t0, tend, lat, lon, alt, elevationMask, step);

    for (int i = 0; i < contacts.size(); i++) {
        std::cout << time2string(contacts[i].AOS) << " " << time2string(contacts[i].LOS) << " " << contacts[i].max_el << "\n" ;
    }
    

    return 0;
}
