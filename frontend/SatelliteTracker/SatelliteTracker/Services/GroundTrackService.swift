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
        
        // Check which type of input is provided: TLE, NAME, CONSTELLATION or MISSION
        switch(inputType) {
        case InputOptions.tle:
            // Parse the string to TLE object
            tle.line2 = String(searchItem.suffix(69))
            tle.line1 = String(searchItem.dropLast(70).suffix(69))
            tle.name = String(searchItem.dropLast(139)).trimmingCharacters(in: .whitespaces)
            tle.startTime = formatter.string(from: start)
            tle.endTime = formatter.string(from: end)
            tle.stepInterval = step
            print(tle)
            // Request
            var request = URLRequest(url: URL(string: "http://172.20.10.13:8080/groundtrack/tle")!)
            request.httpMethod = "POST"
            request.setValue("application/json", forHTTPHeaderField: "Content-Type")
            // Converting input (TLE) to JSON for the server
            request.httpBody = try JSONEncoder().encode(tle) // from swift to JSON and then to server
            let (data, _) = try await URLSession.shared.data(for:request)
            print(String(data: data, encoding: .utf8) ?? "no data")
            return try JSONDecoder().decode([String: GroundTrackPoint].self, from: data)
        case InputOptions.name:
            // Parse the Name object (if spaces are present)
            let parsedName = searchItem.replacingOccurrences(of: " ", with: "+")
            // Request
            var request = URLRequest(url: URL(string: "http://172.20.10.13:8080/groundtrack/name")!)
            request.httpMethod = "POST"
            request.setValue("application/json", forHTTPHeaderField: "Content-Type")
            // Converting input (name) to JSON for the server
            request.httpBody = try JSONEncoder().encode(parsedName) // from swift to JSON and then to server
            let (data, _) = try await URLSession.shared.data(for:request)
            return try JSONDecoder().decode([String: GroundTrackPoint].self, from: data)
        case InputOptions.constellation:
            // call function to provide input as constellation to the main propagator
            print("constellation")
            throw GroundTrackError.notImplemented
        case InputOptions.mission:
            // call function to provide input as mission to the main propagator
            print("mission")
            throw GroundTrackError.notImplemented
        }
        
     
    }
    
}
