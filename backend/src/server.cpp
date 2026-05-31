#include "../external/httpLib/httplib.h"
#include "../external/httpLib/json.hpp"
#include "domain/Tle.h"
#include "domain/sgp4Propagator.h"

// ========================

int main() {

    httplib::Server svr;

    svr.Post("/groundtrack/tle", [](const httplib::Request& req, httplib::Response& res) {

        auto body = nlohmann::json::parse(req.body);

        // Input type: TLE
        Tle tle;
        tle.name = body["name"].get<std::string>();
        tle.line1 = body["line1"].get<std::string>();
        tle.line2 = body["line2"].get<std::string>();

        // Fetch Start and End timeUTC (from TLE body - JSON)
        std::string startString = body["startTime"].get<std::string>();
        std::string endString = body["endTime"].get<std::string>();

        // Initialise the TimeUTC object and fetch components
        TimeUTC start;
        TimeUTC end;
        // ------------------
        start.year = std::stoi(startString.substr(0,4));
        start.month = std::stoi(startString.substr(5,2));
        start.day = std::stoi(startString.substr(8,2));
        start.hour = std::stoi(startString.substr(11,2));
        start.minute = std::stoi(startString.substr(14,2));
        start.second = std::stod(startString.substr(17,2));
        // ------------------
        end.year = std::stoi(endString.substr(0,4));
        end.month = std::stoi(endString.substr(5,2));
        end.day = std::stoi(endString.substr(8,2));
        end.hour = std::stoi(endString.substr(11,2));
        end.minute = std::stoi(endString.substr(14,2));
        end.second = std::stod(endString.substr(17,2));

        // Get the step from the user (if any)
        std::string stepString = body["stepInterval"].get<std::string>();
        double step;
        if (stepString != "") {
            step = std::stod(stepString);
        } else {
            step = 60.0 / 60.0; // default value: 1 min [min]
        }

        // Use function propagate() to get the grountrack object
        std::vector<GroundTrack> gt = propagate(tle, start, end, step);
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

        std::cout << res.body;

    });

    svr.Post("/groundtrack/name", [](const httplib::Request& req, httplib::Response& res) {
        auto body = nlohmann::json::parse(req.body);

        // Input type: Satellite Name (already parsed with spaces removed)
        std::string satName = body["name"].get<std::string>();

        // Celestrak URL using NAME as research
        std::string urlName = "/NORAD/elements/gp.php?NAME=" + satName + "&FORMAT=tle";

        // Fetch TLE from Celestrak client
        httplib::Client cli("http://celestrak.org");
        auto cliRes = cli.Get(urlName);
        auto rawTle = cliRes->body;
        std::cout << "RAW TLE: [" << rawTle << "]" << std::endl;

        // Parsing TLE from celestrak
        Tle tle;
        std::istringstream stream(rawTle);
        std::getline(stream, tle.name);
        std::getline(stream, tle.line1);
        std::getline(stream, tle.line2);
        tle.name.erase(tle.name.find_last_not_of(" \t\r\n") + 1);
        tle.line1.erase(tle.line1.find_last_not_of(" \t\r\n") + 1);
        tle.line2.erase(tle.line2.find_last_not_of(" \t\r\n") + 1);

        std::cout << tle.line1;

        // Fetch Start and End timeUTC (from TLE body - JSON)
        std::string startString = body["startTime"].get<std::string>();
        std::string endString = body["endTime"].get<std::string>();

        // Initialise the TimeUTC object and fetch components
        TimeUTC start;
        TimeUTC end;
        // ------------------
        start.year = std::stoi(startString.substr(0,4));
        start.month = std::stoi(startString.substr(5,2));
        start.day = std::stoi(startString.substr(8,2));
        start.hour = std::stoi(startString.substr(11,2));
        start.minute = std::stoi(startString.substr(14,2));
        start.second = std::stod(startString.substr(17,2));
        // ------------------
        end.year = std::stoi(endString.substr(0,4));
        end.month = std::stoi(endString.substr(5,2));
        end.day = std::stoi(endString.substr(8,2));
        end.hour = std::stoi(endString.substr(11,2));
        end.minute = std::stoi(endString.substr(14,2));
        end.second = std::stod(endString.substr(17,2));

        // Get the step from the user (if any)
        std::string stepString = body["stepInterval"].get<std::string>();
        double step;
        if (stepString != "") {
            step = std::stod(stepString);
        } else {
            step = 60.0 / 60.0; // default value: 1 min [min]
        }

        std::vector<GroundTrack> gt = propagate(tle, start, end, step);
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