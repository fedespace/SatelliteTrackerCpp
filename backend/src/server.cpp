#include "../external/httpLib/httplib.h"
#include "../external/httpLib/json.hpp"
#include "domain/Tle.h"
#include "domain/sgp4Propagator.h"
#include "domain/PassPrediction.h"

// ========================

int main() {

    httplib::Server svr;

    svr.Post("/groundtrack/tle", [](const httplib::Request& req, httplib::Response& res) {

        auto body = nlohmann::json::parse(req.body);

        // Fetch from body
        Tle tle;
        tle.name = body["name"].get<std::string>();
        tle.line1 = body["line1"].get<std::string>();
        tle.line2 = body["line2"].get<std::string>();
        std::string startString = body["startTime"].get<std::string>();
        std::string endString = body["endTime"].get<std::string>();
        std::string stepString = body["stepInterval"].get<std::string>();
        double step;
        if (stepString != "") {
            step = std::stod(stepString);
        } else {
            step = 60.0 / 60.0; // default value: 1 min [min]
        }

        // Convert times in TimeUTC objects
        TimeUTC start = string2time(startString);
        TimeUTC end = string2time(endString);

        // Use function propagate() to get the grountrack object
        std::vector<GroundTrack> gt = propagate(tle, start, end, step);
        std::vector<double> lat_vec, lon_vec;
        for (int i = 0; i < gt.size(); i++) {
            lat_vec.push_back(gt[i].lat);
            lon_vec.push_back(gt[i].lon);
        }

        // Serialize the result to JSON
        nlohmann::json result = serialize_gt(gt);

        res.set_content(result.dump(), "application/json");
    });

    svr.Post("/groundtrack/name", [](const httplib::Request& req, httplib::Response& res) {
        auto body = nlohmann::json::parse(req.body);
       
        // Input type: Satellite Name (already parsed with spaces removed)
        std::string satName = body["name"].get<std::string>();
        Tle tle = fetchTLE("name", satName);

        // Get other inputs from frontend
        std::string startString = body["startTime"].get<std::string>();
        std::string endString = body["endTime"].get<std::string>();
        std::string stepString = body["stepInterval"].get<std::string>();
        double step;
        if (stepString != "") {
            step = std::stod(stepString);
        } else {
            step = 60.0 / 60.0; // default value: 1 min [min]
        }

        // Convert times in TimeUTC objects
        TimeUTC start = string2time(startString);
        TimeUTC end = string2time(endString);

        // Use function propagate() to get the grountrack object
        std::vector<GroundTrack> gt = propagate(tle, start, end, step);
        std::vector<double> lat_vec, lon_vec;
        for (int i = 0; i < gt.size(); i++) {
            lat_vec.push_back(gt[i].lat);
            lon_vec.push_back(gt[i].lon);
        }

        // Serialize the result to JSON
        nlohmann::json result = serialize_gt(gt);

        res.set_content(result.dump(), "application/json");
    });

    svr.Post("/groundtrack/norad", [](const httplib::Request& req, httplib::Response& res) {
        auto body = nlohmann::json::parse(req.body);
       
        std::string noradID = body["norad"].get<std::string>();
        Tle tle = fetchTLE("norad", noradID);

        // Get other inputs from frontend
        std::string startString = body["startTime"].get<std::string>();
        std::string endString = body["endTime"].get<std::string>();
        std::string stepString = body["stepInterval"].get<std::string>();
        double step;
        if (stepString != "") {
            step = std::stod(stepString);
        } else {
            step = 60.0 / 60.0; // default value: 1 min [min]
        }

        // Convert times in TimeUTC objects
        TimeUTC start = string2time(startString);
        TimeUTC end = string2time(endString);

        // Use function propagate() to get the grountrack object
        std::vector<GroundTrack> gt = propagate(tle, start, end, step);
        std::vector<double> lat_vec, lon_vec;
        for (int i = 0; i < gt.size(); i++) {
            lat_vec.push_back(gt[i].lat);
            lon_vec.push_back(gt[i].lon);
        }

        // Serialize the result to JSON
        nlohmann::json result = serialize_gt(gt);

        res.set_content(result.dump(), "application/json");
    });

    svr.Post("/gs/passes", [](const httplib::Request& req, httplib::Response& res) {
        auto body = nlohmann::json::parse(req.body);

        Tle tle;
        tle.name = body["name"].get<std::string>();
        tle.line1 = body["line1"].get<std::string>();
        tle.line2 = body["line2"].get<std::string>();
        std::string startString = body["startTime"].get<std::string>();
        std::string endString = body["endTime"].get<std::string>();
        double gsLat = std::stod(body["gsLat"].get<std::string>());
        double gsLon = std::stod(body["gsLon"].get<std::string>());
        double gsAlt = std::stod(body["gsAlt"].get<std::string>());
        double gsMask = std::stod(body["gsMask"].get<std::string>());

        TimeUTC start = string2time(startString); // [UTC]
        TimeUTC end = string2time(endString); // [UTC]

        std::vector<PassPrediction> passes = passTimes(tle, start, end, gsLat, gsLon, gsAlt, gsMask, 5.0);

        std::string aos;
        std::string los;
        nlohmann::json result;

        for (int i = 0; i < passes.size(); i++) {
            int id = passes[i].id; 
            result[id]["id"] = id;
            aos = time2string(passes[i].AOS);
            aos.substr(0, aos.length() - 7);
            result[id]["aos"] = aos;
            los = time2string(passes[i].LOS);
            los.substr(0, los.length() - 7);
            result[id]["los"] = time2string(passes[i].LOS);
            result[id]["maxEl"] = std::to_string(passes[i].max_el);
            result[id]["duration"] = std::to_string(passes[i].duration);
            result[id]["quality"] = passes[i].qos;
            result[id]["passIsVisible"] = passes[i].passIsVisible;
            }
        
        res.set_content(result.dump(), "application/json");
    
    });

    std::cout << "Server listening on port 8080" << std::endl;
    svr.listen("0.0.0.0", 8080);
}