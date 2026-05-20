#include "Plot2D.h"

// void plot2D(
//     const std::vector<double>& x,
//     const std::vector<double>& y,
//     const char* title)
//     {
//         if (x.size() != y.size() || x.empty())
//             return;


// //             TGraph *gr = new TGraph(n, x, y);
// // gr->Draw("AC*"); // Draw first to initialize axes
// // gr->GetXaxis()->SetTitle("X-Axis Title");
// // gr->GetXaxis()->SetLimits(0.0, 10.0); // Set range from 0 to 10
// // c1->Update(); // Update canvas to reflect changes

//         TGraph *g = new TGraph(x.size(), x.data(), y.data());
//         TCanvas c("c", title, 800, 600);
//         g->SetTitle(title);
//         g->SetMarkerStyle(20);
//         g->SetMinimum(-90); // on Y axis
//         g->SetMaximum(90); // on Y axis
//         g->GetXaxis()->SetLimits(-180.0, 180.0);

//         g->Draw("AL");

//         c.Update();

//         c.SaveAs("plot.png");   // or remove if interactive wanted
//     }