//
//  PassService.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 02/08/2026.
//

import SwiftUI

struct PassService {
    
    var chosenIP = IPaddress.genericIP.rawValue
    @Binding var inputType: InputOptions?
    @Binding var searchItem: String?
    
    func fetchPasses(satellite: String, startTime: Date, endTime: Date, gsLat: String, gsLon: String, gsAlt: String, gsMask: String, inputType: InputOptions, searchItem: String) async throws -> [Pass] {
        
        let formatter = ISO8601DateFormatter()
        formatter.timeZone = TimeZone(identifier: "UTC")!
        let startFormatted = formatter.string(from: startTime) // this is translated in UTC time
        let endFormatted = formatter.string(from: endTime)
        
        // Case TLE is provided, avoid fetching again from Celestrak
        if inputType == .tle {
            var tle = PassTLERequest()
            tle.line2 = String(searchItem.suffix(69))
            tle.line1 = String(searchItem.dropLast(70).suffix(69))
            tle.name = String(searchItem.dropLast(139)).trimmingCharacters(in: .whitespaces)
            tle.startTime = startFormatted
            tle.endTime = endFormatted
            tle.gsLat = gsLat
            tle.gsLon = gsLon
            tle.gsAlt = gsAlt
            tle.gsMask = gsMask
            
            var request = URLRequest(url: URL(string: "http://\(chosenIP):8080/gs/tlePass")!)
            request.httpMethod = "POST"
            request.setValue("application/json", forHTTPHeaderField: "Content-Type")
            request.httpBody = try JSONEncoder().encode(tle)
            let (data, _) = try await URLSession.shared.data(for:request)
            print(String(data: data, encoding: .utf8) ?? "no data")
            return try JSONDecoder().decode([Pass].self, from: data)
        } else {
            var req = PassRequest()
            req.satellite = satellite
            req.startTime = startFormatted
            req.endTime = endFormatted
            req.gsLat = gsLat
            req.gsLon = gsLon
            req.gsAlt = gsAlt
            req.gsMask = gsMask
            
            var request = URLRequest(url: URL(string: "http://\(chosenIP):8080/gs/passes")!)
            request.httpMethod = "POST"
            request.setValue("application/json", forHTTPHeaderField: "Content-Type")
            request.httpBody = try JSONEncoder().encode(req)
            let (data, _) = try await URLSession.shared.data(for:request)
            print(String(data: data, encoding: .utf8) ?? "no data")
            return try JSONDecoder().decode([Pass].self, from: data)
        }
        
    }
}
