#include <iostream>
#include <iomanip>
#include "domain/sgp4Propagator.h"
#include "./domain/PassPrediction.h"

// ===============================

int main() {
    
    // Get ECEF from ground station coordinates
    double lat = 51.53;
    double lon = 0.34;
    double alt = 29.0;
    double elevationMask = 10.0;
    double step = 5.0; // [sec]

    Tle iss;
    iss.name = "ISS (ZARYA)";   
    iss.line1 = "1 25544U 98067A   26227.08368470  .00004985  00000+0  97076-4 0  9993";
    iss.line2 = "2 25544  51.6331   8.6030 0007568  47.4901 312.6726 15.49446860580882";

    TimeUTC t0 = {2026, 8, 19, 0, 0, 0.0};
    TimeUTC tend = {2026, 8, 21, 0, 0, 0.0};

    std::vector<PassPrediction> contacts = passTimes(iss, t0, tend, lat, lon, alt, elevationMask, step);

    for (int i = 0; i < contacts.size(); i++) {
        std::cout << "From: " << time2string(contacts[i].AOS) << " to: " << time2string(contacts[i].LOS) << ", with duration: " << contacts[i].duration << "\n"; // << " " << time2string(contacts[i].LOS) << " " << contacts[i].max_el << "\n" ;
    }
    

    return 0;
}
