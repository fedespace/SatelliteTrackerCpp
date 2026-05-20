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

    }
    
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what();
        return 1;
    }
    return 0;
}
