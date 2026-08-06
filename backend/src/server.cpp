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

        // Celestrak URL using NAME as research
        std::string urlName = "/NORAD/elements/gp.php?NAME=" + satName + "&FORMAT=tle";

        // Fetch TLE from Celestrak client
        httplib::Client cli("http://celestrak.org");
        auto cliRes = cli.Get(urlName);
        auto rawTle = cliRes->body;

        // Fetch other inputs and parse TLE from Celestrak
        Tle tle;
        std::istringstream stream(rawTle);
        std::getline(stream, tle.name);
        std::getline(stream, tle.line1);
        std::getline(stream, tle.line2);
        tle.name.erase(tle.name.find_last_not_of(" \t\r\n") + 1);
        tle.line1.erase(tle.line1.find_last_not_of(" \t\r\n") + 1);
        tle.line2.erase(tle.line2.find_last_not_of(" \t\r\n") + 1);
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

        // Input type: Satellite NORAD ID
        std::string noradID = body["norad"].get<std::string>();

        // Celestrak URL using NAME as research
        std::string urlName = "/NORAD/elements/gp.php?CATNR=" + noradID + "&FORMAT=tle";

        // Fetch TLE from Celestrak client
        httplib::Client cli("http://celestrak.org");
        auto cliRes = cli.Get(urlName);
        auto rawTle = cliRes->body;

        // Fetch other inputs and parse TLE from Celestrak
        Tle tle;
        std::istringstream stream(rawTle);
        std::getline(stream, tle.name);
        std::getline(stream, tle.line1);
        std::getline(stream, tle.line2);
        tle.name.erase(tle.name.find_last_not_of(" \t\r\n") + 1);
        tle.line1.erase(tle.line1.find_last_not_of(" \t\r\n") + 1);
        tle.line2.erase(tle.line2.find_last_not_of(" \t\r\n") + 1);
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

    svr.Post("/groundtrack/default", [](const httplib::Request& req, httplib::Response& res) {
        auto body = nlohmann::json::parse(req.body);
    
        // Celestrak URL using NAME as research
        std::string urlName = "/NORAD/elements/gp.php?NAME=" + body["satellite"].get<std::string>() + "&FORMAT=tle";

        // Fetch TLE from Celestrak client
        httplib::Client cli("http://celestrak.org");
        cli.set_interface("utun7");
        auto cliRes = cli.Get(urlName);
        auto rawTle = cliRes->body;

        // Fetch other inputs and parse TLE from Celestrak
        Tle tle;
        std::istringstream stream(rawTle);
        std::getline(stream, tle.name);
        std::getline(stream, tle.line1);
        std::getline(stream, tle.line2);
        tle.name.erase(tle.name.find_last_not_of(" \t\r\n") + 1);
        tle.line1.erase(tle.line1.find_last_not_of(" \t\r\n") + 1);
        tle.line2.erase(tle.line2.find_last_not_of(" \t\r\n") + 1);
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

        /*
        ISS (ZARYA)             
1 25544U 98067A   26217.02190500  .00005828  00000+0  11248-3 0  9996
2 25544  51.6318  58.4155 0007310  13.5287 346.5896 15.49349819579321
        */


        auto body = nlohmann::json::parse(req.body);

        //std::cout << "body from swift: " << body << "\n";

        // Input type: Satellite NORAD ID
        std::string noradID = body["satellite"].get<std::string>();

        // Celestrak URL using NAME as research
        std::string urlName = "/NORAD/elements/gp.php?CATNR=" + noradID + "&FORMAT=tle";

        // Fetch TLE from Celestrak client
        httplib::Client cli("http://celestrak.org");
        auto cliRes = cli.Get(urlName);
        auto rawTle = cliRes->body;

        // Fetch other inputs and parse TLE from Celestrak
        Tle tle;
        std::istringstream stream(rawTle);
        std::getline(stream, tle.name);
        std::getline(stream, tle.line1);
        std::getline(stream, tle.line2);
        tle.name.erase(tle.name.find_last_not_of(" \t\r\n") + 1);
        tle.line1.erase(tle.line1.find_last_not_of(" \t\r\n") + 1);
        tle.line2.erase(tle.line2.find_last_not_of(" \t\r\n") + 1);
        std::string startString = body["startTime"].get<std::string>();
        std::string endString = body["endTime"].get<std::string>();

        //std::cout << "Name: " << tle.name << ", line 1: " << tle.line1 << ", line 2: " << tle.line2 << "\n"; 

        TimeUTC start = string2time(startString); // [UTC]
        TimeUTC end = string2time(endString); // [UTC]

        //std::cout << "Time start and time end from swift: " << time2string(start) << " " << time2string(end) << "\n";

        // Fetch other inputs
        double gsLat = std::stod(body["gsLat"].get<std::string>());
        double gsLon = std::stod(body["gsLon"].get<std::string>());
        double gsAlt = std::stod(body["gsAlt"].get<std::string>());
        double gsMask = std::stod(body["gsMask"].get<std::string>());

        //std::cout << "Lat lon alt mask from swift: " << gsLat << " " << gsLon << " " << gsAlt << " " << gsMask << "\n";

        std::vector<PassPrediction> passes = passTimes(tle, start, end, gsLat, gsLat, gsAlt, gsMask, 5.0);

        // for (int i = 0; i < passes.size(); i++) {
        // //std::cout << time2string(contacts[i].AOS); // << " " << time2string(contacts[i].LOS) << " " << contacts[i].max_el << "\n" ;
        //     std::cout << passes[i].AOS.second << "\n";
        // }

        std::string aos;
        std::string los;
        nlohmann::json result;

        for (int i = 0; i < passes.size(); i++) {
            int id = passes[i].id; 
            result[id]["id"] = id;
            aos = time2string(passes[i].AOS);
            aos.substr(0, aos.length() - 7);
            result[id]["aos"] = aos;
            //std::cout << "AOS sent to swift: " << aos << "\n";
            los = time2string(passes[i].LOS);
            los.substr(0, los.length() - 7);
            result[id]["los"] = time2string(passes[i].LOS);
            //std::cout << "LOS sent to swift: " << los << "\n";
            result[id]["maxEl"] = std::to_string(passes[i].max_el);
            result[id]["duration"] = std::to_string(passes[i].duration);
            result[id]["quality"] = "";
            }
        
        res.set_content(result.dump(), "application/json");

    });

    std::cout << "Server listening on port 8080" << std::endl;
    svr.listen("0.0.0.0", 8080);
}
        