#include <iostream>
#include "domain/Tle.h"
#include "frames/Teme2Ecef.h"
#include "frames/FrameTransforms.h"
#include "domain/GroundTrack.h"
#include <iomanip>
#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TApplication.h"

// ===============================

int main() {
    
    try {
        
        // -------------------------------
        
        // Define the object TLE of type Tle, validate and standardise
        Tle tle = {
            "ONEWEB-0162",
            "1 48056U 21025Q   26046.24572298 -.00000136  00000+0 -40788-3 0  9991",
            "2 48056  87.9044 344.4620 0001950  75.0816 285.0529 13.13470762237537"
        };
        validate(tle);
        standardise(tle);
        
        // Start & End time (user input)
        TimeUTC timeS = {2026, 2, 16, 1, 0, 0.0};
        validate(timeS);
        TimeUTC timeE = {2026, 2, 16, 5, 30, 0.0};
        validate(timeE);
        
        // Fetch param from TLE
        TleParam parameters = parseTLE(tle);
        gravconsttype WGS = parameters.whichconst;
        char opsmode = parameters.opsmode;
        std::array<char,6> satn = parameters.satn;
        double epoch = parameters.epoch;
        double bstar = parameters.bstar;
        double ndot = parameters.ndot;
        double nddot = parameters.nddot;
        double ecco = parameters.ecco;
        double argpo = parameters.argpo;
        double inclo = parameters.inclo;
        double mo = parameters.mo;
        double no_kozai = parameters.no_kozai;
        double nodeo = parameters.nodeo;
        
         // Init of the structure "satrec"
        elsetrec satrec;
        SGP4Funcs::sgp4init (
            WGS, opsmode, satn.data(), epoch, bstar, ndot, nddot, ecco, argpo, inclo, mo, no_kozai, nodeo, 
            satrec
        );

        // Conversion of timeUTC to "time since epoch (minutes)"
        double timeS_min = epoch2mins(timeS);
        double timeE_min = epoch2mins(timeE);
        double deltaT = timeE_min - timeS_min;

        // While loop for the propagation and grountrack
        double step = 60.0 / 60.0; // [min] considering now 1 minute at a time
        double tsince = step; // in the first iteration that's equal to the step
        double r[3], v[3];
        double jd, jdfrac;
        double t = timeS_min; // the time that will keep incrementing in the groundtrack loop
        double radius_earth = satrec.radiusearthkm;


        // std::cout << std::setprecision(10) << timeS_min << "\n"; //MJD20002epoch(timeS_min).day;
        std::cout << MJD20002epoch(timeS_min).year << "\n";
        std::cout << MJD20002epoch(timeS_min).month << "\n";
        std::cout << MJD20002epoch(timeS_min).day << "\n";
        std::cout << MJD20002epoch(timeS_min).hour << "\n";
        std::cout << MJD20002epoch(timeS_min).minute << "\n";
        std::cout << MJD20002epoch(timeS_min).second << "\n";

        double lat, lon;
        std::vector<double> lat_vec, lon_vec;
        std::vector<GroundTrack> gt;
        while (tsince <= deltaT) {
            SGP4Funcs::sgp4(satrec, tsince, r, v);
            double t1 = t + step; // [min]
            TimeUTC t1_components = MJD20002epoch(t1); // [year, month, day, hour, minute, seconds]
            SGP4Funcs::jday_SGP4(
                t1_components.year, t1_components.month, t1_components.day, t1_components.hour, t1_components.minute, t1_components.second,
                jd, jdfrac
            );
            double jd_full = jd + jdfrac;
            double gstime = SGP4Funcs::gstime_SGP4(jd_full); // [rad]
            Matrix3x3 R = rotationMatrixZ(gstime);
            Vector3D r_teme = {r[0], r[1], r[2]};
            Vector3D r_ecef = rotateZ(R, r_teme);
            ecef2ll(r_ecef, radius_earth, lat, lon);
            lat_vec.push_back(lat);
            lon_vec.push_back(lon);
            GroundTrack single_point = {t1_components, lat, lon};
            gt.push_back(single_point);
            
            // Update t for next iteration
            t += step; 
            tsince += step;

            // Print for checks
            // std::cout << "Time of propagation: " << t1 << "\n";
            // std::cout << "TEME vector: " << r_teme.x << " " << r_teme.y << " " << r_teme.z << "\n";
            // std::cout << "ECEF vector: " << r_ecef.x << " " << r_ecef.y << " " << r_ecef.z << "\n";
            // std::cout << "Lat / Lon: " << lat << ", " << lon << "\n";

        }
        
        // Plotting the result
        TApplication app("app", nullptr, nullptr);
        TGraph* graph = new TGraph(lon_vec.size(), lon_vec.data(), lat_vec.data());
        TCanvas* c = new TCanvas("c", "Ground Track", 800, 600);
        graph->SetMarkerStyle(20);
        graph->Draw("APL");
        graph->GetXaxis()->SetLimits(-180, 180);
        graph->GetYaxis()->SetRangeUser(-90, 90);
        graph->SetTitle("Ground Track;Longitude;Latitude");

        c->Update();
        app.Run();

        // printTableGT(gt);

    }
        // -------------------------------

        // Define the start time and end time of type TimeUTC and separate the components
        // This will be an input provided by the user together the TLE
//        int yS = 2026;
//        int monS = 2;
//        int dS = 16;
//        int hS = 1;
//        int miS = 0;
//        double sS = 0.0;
//        TimeUTC timeS = {yS, monS, dS, hS, miS, sS};
//        validate(timeS);
//
//        int yE = 2026;
//        int monE = 2;
//        int dE = 16;
//        int hE = 5;
//        int miE = 30;
//        double sE = 0.0;
//        TimeUTC timeE = {yE, monE, dE, hE, miE, sE};
//        validate(timeE);
//
//        // -------------------------------
//
//        // Defining values to be used in sgp4init
//        TleParam par = parseTLE(tle);
//
//        // -------------------------------
//
//        // Initialize the structure elsetrec with var name "satrec"
//        elsetrec satrec;
//
//        // Calling the sgp4init function from Vallado's library
//        SGP4Funcs::sgp4init (
//		    par.whichconst, 
//            par.opsmode, 
//            par.satn.data(), 
//            par.epoch, 
//            par.bstar, 
//            par.ndot, 
//            par.nddot, 
//            par.ecco, 
//            par.argpo,
//		    par.inclo, 
//            par.mo, 
//            par.no_kozai,
//		    par.nodeo,
//            satrec
//		);
//
//        // -------------------------------
//
//        // Defining tStart and tEnd of propagation in [s]
//        double tStart = epoch2mins(timeS) * 60.0; // [s]
//        double tEnd = epoch2mins(timeE) * 60.0; // [s]
//
//        // Defining starting time of the propagation for the iteration as t
//        double t = tStart; // [s]
//
//        // -------------------------------
//
//        // Running the iteration till tEnd
//        std::vector<double> lat;
//        std::vector<double> lon;
//        double step = 60.0; // [s]
//        while (t <= tEnd) {
//            double tTLE = epoch2mins_TLE(tle); // [m]
//            double tsince = t / 60.0 - tTLE; // [m]
//            double r[3], v[3];
//            SGP4Funcs::sgp4(satrec, tsince, r, v);
//            if (satrec.error != 0) {
//                std::cout << "Error rising from propagation function.\n";
//            } else {
//                Vector3D rteme = {r[0], r[1], r[2]};
//                double jd, jdF;
//                TimeUTC tEpoch = MJD20002epoch(t / 60.0);
//                SGP4Funcs::jday_SGP4(tEpoch.year, tEpoch.month, tEpoch.day, tEpoch.hour, tEpoch.minute, tEpoch.second, jd ,jdF);
//                double jdut1 = jd + jdF;
//                double gstm = SGP4Funcs::gstime_SGP4(jdut1); // [rad]
//                Matrix3x3 R = rotationMatrixZ(gstm);
//                Vector3D recef = rotateZ(R, rteme);
//                LL ll = ecef2ll(recef);
//                lat.push_back(ll.lat);
//                lon.push_back(ll.lon);
//                t = t + step;
//            }
//        }
//
//        // -------------------------------
//
//        // Printing the table with values
//        std::cout << "Time start: " << yS << ", " << monS << ", " << dS << ", " << hS << ", " << miS << ", " << sS << "\n";
//        std::cout << "Time end: " << yE << ", " << monE << ", " << dE << ", " << hE << ", " << miE << ", " << sE << "\n";
//
//        // Header justified on the left with columns of 15, 10, 10 width
//        std::cout << std::left
//                << std::setw(10) << "Latitude"
//                << std::setw(10)  << "Longitude"
//                << "\n";
//
//        // Divider between header and data
//        std::cout << std::string(35, '=') << "\n";
//
//        // Rows
//        for (int i = 0; i < lat.size(); i++){
//            std::cout << std::left
//                    << std::setw(10) << lat[i]
//                    << std::setw(10)  << lon[i]
//                    << "\n";
//        }
//    }
//
//
//        // Computing tsince, from TLE epoch to endpoint, in minutes
//        // double tleMins = epoch2mins_TLE(tle);
//        // double timeMins = epoch2mins(time);
//        // double tsince = timeMins - tleMins;
//
//        // -------------------------------
//
//        // // Brief check to see if tsince > 0 hence timeMins > tleMins
//        // if (tsince >= 0.0) {
//        //     // Initiating position and velocity vecs
//        //     double r[3], v[3];
//        //     // Calling sgp4 propagator using satrec strucure
//        //     SGP4Funcs::sgp4(satrec, tsince, r, v);
//
//        //     // Check if the propagation reported errors (check which one in sgp4.cpp code)
//        //     if (satrec.error != 0) {
//        //         std::cout << "Error rising from propagation function.\n";
//        //     } else {
//        //         // Definition of rTEME as Vector3D
//        //         Vector3D rteme = {r[0], r[1], r[2]};
//
//        //         // Computation of jd and gstm (Vallado's functions)
//        //         std::cout << std::setprecision(25) << y << ", " << mon << ", " << d << ", " << h << ", " << mi << ", " << s << "\n";
//        //         double jd, jdF;
//        //         SGP4Funcs::jday_SGP4(y, mon, d, h, mi, s, jd ,jdF);
//        //         double jdut1 = jd + jdF;
//        //         double gstm = SGP4Funcs::gstime_SGP4(jdut1); // [rad]
//                
//        //         // Create the rotation matrix
//        //         Matrix3x3 R = rotationMatrixZ(gstm);
//                
//        //         // Rotate rteme using R and find recef
//        //         Vector3D recef = rotateZ(R, rteme);
//
//        //         // Apply function to get the latitude and longitude from recef
//        //         LL ll = ecef2ll(recef);
//
//        //         std::cout << std::setprecision(25) << rteme.x << ", " << rteme.y << ", " << rteme.z << "\n";
//        //         std::cout << std::setprecision(25) << jd << ", " << jdF << ", " << jdut1 << ", " << gstm << "\n";
//        //         std::cout << std::setprecision(25) << recef.x << ", " << recef.y << ", " << recef.z << "\n";
//        //         std::cout << std::setprecision(25) << ll.lat << ", " << ll.lon << "\n";
//        //     }
//
//        // }
//
//        // Compute the type of orbit based on TLE and dt validity
//        // elsetrec satrec{}; 
//        // OrbitClassif classification = typeOfOrbit(twoLineElements, satrec.mus);
//        // double validity = 1440*15; // default (GEO): 2 weeks
//        // if (classification == OrbitClassif::HEO || classification == OrbitClassif::GTO || classification == OrbitClassif::MEO) {
//        //     validity = 1440*2; // 2 days
//        // } else {validity = 1440;} // single day for LEO
//        // std::cout << "Validity period: " << validity << std::endl;
//
//        // CASE 1 - SINGLE POINT @ TARGET TIME REQUEST
//        // This will be after frontend implementation: if "!endTime" > single
//        // else if "endTime" (and step - optional) > groundtrack
//        // double mjd_target = epoch2mins(target);
//        // int yearTLE = std::stoi(twoLineElements.line1.substr(18,2)) + 2000; 
//        // double dayOfYearTLE = std::stod(twoLineElements.line1.substr(20,12));
//        // double mjd_TLE = epoch2mins_TLE(yearTLE, dayOfYearTLE);
//        // double d = mjd_target - mjd_TLE;
//        // //std::cout << "Times: " << std::setprecision(10) << mjd_target << " " << yearTLE << " " << dayOfYearTLE << " " << std::setprecision(10) << mjd_TLE << " " << d << std::endl;
//        // if (d < validity) {
//        //     GroundTrack singlePoint = point(twoLineElements, target);
//        //     std::cout << singlePoint.lat << " " << singlePoint.lon << std::endl;
//        // } else {
//        //     std::cout << "Aborting...";
//        // }
//
//        // CASE 2 - GROUNDTRACK FROM START TIME TO END TIME WITH STEP [s]
//        // Example with end date at 10:00 AM and 30s of step
//        //LL ll = ecef2ll({4512.3, -3210.7, 4021.8});
//        //TimeUTC t = {2026,2,16,1,0,0.0};
//
//        
//        // Other rECEF vectors for validation: 
//        // r_ECEF = {6378.137, 0.0, 0.0}; // expected [lat 0, lon 0]
//        // r_ECEF = {3194.419, 3194.419, 4487.348}; // expected [lat 45, lon -45]
//        // r_ECEF = {961.996, -555.386, 6260.372}; // expected [lat 80, lon -30]
//
//        // Converting r_ECEF to [Lat, Long] pair
//        // LL ll = ecef2ll(r_ECEF);
//        // double lat = ll.lat;
//        // double lon = ll.lon;
//        // std::cout << "Lon, lat [deg]: {" << lon << ", " << lat << "}\n";
//
//        // Setting the step interval for plotting the groundtrack
//        // OrbitClassif classification = rv.classification;
//        // double gStep;
//        // if (!gStep) {
//        //     if (classification == OrbitClassif::LEO) {
//        //         gStep = 10; // [s]
//        //     } else if (classification == OrbitClassif::MEO) {
//        //         gStep = 30; // [s]
//        //     } else if (classification == OrbitClassif::GTO || classification == OrbitClassif::HEO) {
//        //         gStep = 60; // [s]
//        //     } else {
//        //         gStep = 300; // [s]
//        //     }
//        // }
//
//        // Defining max epochTime from TLE allowed to have validity of the gt
//        // int maxTargetVal = rv.validityTle;
//        // int endGT; 
//        // if (!endGT) {
//        //     endGT = maxTargetVal;
//        // }
//
//        // TEST MJD and TimeUTC
//        // TimeUTC timestart = {2026, 2, 18, 10, 36, 22.0};
//        // std::cout << "Time start: " << timestart.day << " " << timestart.month << " " << timestart.year << " " << timestart.hour << " " << timestart.minute << " " << timestart.second << "\n";
//        // double mjd_timestart = epoch2mins(timestart);
//        // std::cout << "mjd of timestart: " << std::setprecision(10) << mjd_timestart << "\n";
//        // TimeUTC convertedBack = MJD20002epoch(14814720);
//        // std::cout << convertedBack.year << " " << convertedBack.month << " " << convertedBack.day << " " << convertedBack.hour << " " << convertedBack.minute << " " << convertedBack.second << std::endl;
//
//        // Groundtrack plot test
//        // gtPoints(twoLineElements, {2026, 2, 15, 1, 0, 0.0}, {2026, 2, 15, 8, 0, 0.0}, 30);
//        
    
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what();
        return 1;
    }
    return 0;
}
