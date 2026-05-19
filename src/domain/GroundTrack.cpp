#include "GroundTrack.h"
#include <vector>
#include "Plot2D.h"
#include <iomanip>
#include <iostream>
#include <string>

// std::vector<GroundTrack> gtPoints(const Tle& tle, const TimeUTC& start, const TimeUTC& end, double step) {
//     // Initialise the latitude and longitude vector
//     std::vector<GroundTrack> gt;
//     std::vector<double> lat;
//     std::vector<double> lon;
     
//     // Conversion of start and end in MJD2000 seconds
//     double tStartSec = epoch2MJD2000(start) * 60.0; // [s]
//     double tEndSec = epoch2MJD2000(end) * 60.0; // [s]

//     // Definition of SGP4 propagator
//     Sgp4Propagator SGP4_TLE;

//     // Dummy variable t
//     double t = tStartSec;

//     // While cycle, iterating every step
//     while (t <= tEndSec) {

//         TimeUTC tEpoch = MJD20002epoch(t/60.0); 

//         // // Propagation of the orbit at instant t
//         // OrbitState rv = SGP4_TLE.propagate(tle, tEpoch); // from Vallado's library
//         // JD targetJD = SGP4_TLE.jday_SGP4(tEpoch); // from Vallado's library
//         // // Get GMST, used to rotate rv
//         // double gmst = SGP4_TLE.gstime_SGP4(targetJD); // [rad]
//         // // Rotation matrix from TEME to ECEF 
//         // Matrix3x3 rotationMatrix = rotationMatrixZ(gmst);
//         // // Conversion TEME to ECEF
//         // Vector3D r_ECEF = rotateZ(rotationMatrix, rv.position_km);
//         // // Converting r_ECEF to [Lat, Long] pair
//         // LL ll = ecef2ll(r_ECEF);
//         // double latD = ll.lat;
//         // double lonD = ll.lon;
//         // // Store in vectors 
//         // lat.push_back(latD);
//         // lon.push_back(lonD);
//         // // Creation of n+1 grountrack point struct
//         // GroundTrack p = {tEpoch, latD, lonD};
//         // gt.push_back(p);
//         // // Apply the step [sec]
//         // t = t + step;
//     }

//     // Plot the groundtrack after all data is collected
//     std::vector<double> latV, lonV;
//     for (int i = 0; i < gt.size(); i++) {
//         latV.push_back(gt[i].lat);
//         lonV.push_back(gt[i].lon);
//     }
//     // plot2D(latV, lonV, "Groundtrack");

//     // Printing out the table with o
//     // printTableGT(gt);

//     return gt;
// }


// GroundTrack point(const Tle& tle, const TimeUTC& target) {
//     // Definition of SGP4 propagator
//     // Sgp4Propagator SGP4_TLE;
//     // OrbitState rv = SGP4_TLE.propagate(tle, target); // from Vallado's library
//     // JD targetJD = SGP4_TLE.jday_SGP4(target); // from Vallado's library
//     // double gmst = SGP4_TLE.gstime_SGP4(targetJD); // [rad]
//     // Matrix3x3 rotationMatrix = rotationMatrixZ(gmst);
//     // Vector3D r_ECEF = rotateZ(rotationMatrix, rv.position_km);
//     // LL ll = ecef2ll(r_ECEF);

//     return {target, ll.lat, ll.lon};
// }



void printTableGT(const std::vector<GroundTrack> gt) {
    // Header justified on the left with columns of 15, 10, 10 width
    std::cout << std::left
              << std::setw(10) << "Latitude"
              << std::setw(10)  << "Longitude"
              << "\n";

    // Divider between header and data
    std::cout << std::string(35, '=') << "\n";

    // Rows
    for (const auto& groundtrack : gt) {
        std::string time = std::to_string(groundtrack.time.day) + " " + std::to_string(groundtrack.time.month) + " " + std::to_string(groundtrack.time.year) + " " + std::to_string(groundtrack.time.hour) + " " + std::to_string(groundtrack.time.minute) + " " + std::to_string(int(groundtrack.time.second)) + " ";
        std::cout << std::left
                  << std::setw(10) << groundtrack.lat
                  << std::setw(10)  << groundtrack.lon
                  << "\n";
    }
}
