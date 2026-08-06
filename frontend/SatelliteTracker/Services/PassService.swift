//
//  PassService.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 02/08/2026.
//

import SwiftUI

struct PassService {
    
    var chosenIP = IPaddress.genericIP.rawValue
    
    func fetchPasses(satellite: String, startTime: Date, endTime: Date, gsLat: String, gsLon: String, gsAlt: String, gsMask: String) async throws -> [Pass] {
        
        let formatter = ISO8601DateFormatter()
        formatter.timeZone = TimeZone(identifier: "UTC")!
        let startFormatted = formatter.string(from: startTime) // this is translated in UTC time
        let endFormatted = formatter.string(from: endTime)
        
        
        var req = PassRequest()
        req.satellite = satellite
        req.startTime = startFormatted
        req.endTime = endFormatted
        req.gsLat = gsLat
        req.gsLon = gsLon
        req.gsAlt = gsAlt
        req.gsMask = gsMask
        
        
        // Request
        var request = URLRequest(url: URL(string: "http://\(chosenIP):8080/gs/passes")!)
        request.httpMethod = "POST"
        request.setValue("application/json", forHTTPHeaderField: "Content-Type")
        request.httpBody = try JSONEncoder().encode(req)
        let (data, _) = try await URLSession.shared.data(for:request)
        print(String(data: data, encoding: .utf8) ?? "no data")
        return try JSONDecoder().decode([Pass].self, from: data)
    }
    
    
}
