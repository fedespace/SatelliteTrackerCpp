//
//  GroundTrackService.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 22/05/2026.
//

import SwiftUI

enum GroundTrackError: Error {
    case notImplemented
}

struct GroundTrackService {
    
    func fetchGroundTrack(inputType: InputOptions, searchItem: String, start: Date, end: Date, step: String) async throws -> [String: GroundTrackPoint] {
        
        // Initialise the TLE structure and formatter for dates
        var tle = TleRequest()
        let formatter = ISO8601DateFormatter()
        let startFormatted = formatter.string(from: start)
        let endFormatted = formatter.string(from: end)
        
        // Check which type of input is provided: TLE, NAME, CONSTELLATION or MISSION
        switch(inputType) {
        case InputOptions.tle:
            // Parse the string to TLE object
            tle.line2 = String(searchItem.suffix(69))
            tle.line1 = String(searchItem.dropLast(70).suffix(69))
            tle.name = String(searchItem.dropLast(139)).trimmingCharacters(in: .whitespaces)
            tle.startTime = startFormatted
            tle.endTime = endFormatted
            tle.stepInterval = step
            // Request
            var request = URLRequest(url: URL(string: "http://127.0.0.1:8080/groundtrack/tle")!)
            request.httpMethod = "POST"
            request.setValue("application/json", forHTTPHeaderField: "Content-Type")
            // Converting input (TLE) to JSON for the server
            request.httpBody = try JSONEncoder().encode(tle) // from swift to JSON and then to server
            let (data, _) = try await URLSession.shared.data(for:request)
            return try JSONDecoder().decode([String: GroundTrackPoint].self, from: data)
        case InputOptions.name:
            // Parse the Name object (if spaces are present)
            let parsedName = searchItem.replacingOccurrences(of: " ", with: "+")
            let nameSat = NameRequest(name: parsedName, startTime: startFormatted, endTime: endFormatted, stepInterval: step)
            // Request
            var request = URLRequest(url: URL(string: "http://127.0.0.1:8080/groundtrack/name")!)
            request.httpMethod = "POST"
            request.setValue("application/json", forHTTPHeaderField: "Content-Type")
            // Converting input (name) to JSON for the server
            request.httpBody = try JSONEncoder().encode(nameSat) // from swift to JSON and then to server
            let (data, _) = try await URLSession.shared.data(for:request)
            print(String(data: data, encoding: .utf8) ?? "no data")
            return try JSONDecoder().decode([String: GroundTrackPoint].self, from: data)
        case InputOptions.norad:
            let noradReq = NoradRequest(norad: searchItem, startTime: startFormatted, endTime: endFormatted, stepInterval: step)
            var request = URLRequest(url: URL(string: "http://127.0.0.1:8080/groundtrack/norad")!)
            request.httpMethod = "POST"
            request.setValue("application/json", forHTTPHeaderField: "Content-Type")
            request.httpBody = try JSONEncoder().encode(noradReq) // from swift to JSON and then to server
            let (data, _) = try await URLSession.shared.data(for:request)
            print(String(data: data, encoding: .utf8) ?? "no data")
            return try JSONDecoder().decode([String: GroundTrackPoint].self, from: data)
        case InputOptions.iss, InputOptions.hubble:
            var defaultReq = ISS_HUBBLE(startTime: startFormatted, endTime: endFormatted, stepInterval: step)
            if (inputType == InputOptions.iss) {
                defaultReq.satellite = "ISS+(ZARYA)"
            } else {
                defaultReq.satellite = "HST"
            }
            var request = URLRequest(url: URL(string: "http://127.0.0.1:8080/groundtrack/default")!)
            request.httpMethod = "POST"
            request.setValue("application/json", forHTTPHeaderField: "Content-Type")
            request.httpBody = try JSONEncoder().encode(defaultReq)
            let (data, _) = try await URLSession.shared.data(for:request)
            print(String(data: data, encoding: .utf8) ?? "no data")
            return try JSONDecoder().decode([String: GroundTrackPoint].self, from: data)
        }
        
     
    }
    
}
