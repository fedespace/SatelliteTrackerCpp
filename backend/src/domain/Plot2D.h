#pragma once
#include <vector>
#include <string>
// #include "TCanvas.h"
// #include "TGraph.h"
// #include "TAxis.h"

void plot2D(
    const std::vector<double>& x,
    const std::vector<double>& y,
    const char* title = "XY Plot"
);