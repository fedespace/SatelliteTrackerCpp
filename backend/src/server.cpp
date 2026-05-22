#include "../external/httpLib/httplib.h"
#include "../external/httpLib/json.hpp"
#include "domain/Tle.h"
#include "domain/sgp4Propagator.h"

// ========================

int main() {

    httplib::Server svr;

    svr.Post("/groundtrack", [](const httplib::Request& req, httplib::Response& res) {

        auto body = nlohmann::json::parse(req.body);

        Tle tle;
        tle.name = body["name"].get<std::string>();
        tle.line1 = body["line1"].get<std::string>();
        tle.line2 = body["line2"].get<std::string>();

        // For now let's harcode input timeS, timeE. Then will be added to the body of the request.
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

        // Serialize the result to JSON
        nlohmann::json result;
        std::vector<std::string> time_vec;
        for (int i = 0; i < gt.size(); i++) {
            TimeUTC time = gt[i].time;
            std::string time_string = to_iso8601(time);
            result[time_string]["lat"] = gt[i].lat;
            result[time_string]["lon"] = gt[i].lon;
        }

        res.set_content(result.dump(), "application/json");

    });

    std::cout << "Server listening on port 8080" << std::endl;
    svr.listen("0.0.0.0", 8080);
}