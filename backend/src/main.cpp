#include <iostream>
#include <iomanip>
#include "domain/sgp4Propagator.h"
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
    return 0;
}
