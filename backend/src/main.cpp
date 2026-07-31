#include <iostream>
#include <iomanip>
#include "domain/sgp4Propagator.h"
<<<<<<< HEAD
#include "./domain/PassPrediction.h"
=======
#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TApplication.h"
>>>>>>> 2e26dd290322a0e1236dc4ef8338d945021d8130

// ===============================

int main() {
    
<<<<<<< HEAD
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

=======
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
        
        // Use function propagate() to get the grountrack object
        double step = 60.0 / 60.0; // [min] considering now 1 minute at a time
        std::vector<GroundTrack> gt = propagate(tle, timeS, timeE, step);
        std::vector<double> lat_vec, lon_vec;
        for (int i = 0; i < gt.size(); i++) {
            lat_vec.push_back(gt[i].lat);
            lon_vec.push_back(gt[i].lon);
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
>>>>>>> 2e26dd290322a0e1236dc4ef8338d945021d8130
    return 0;
}
