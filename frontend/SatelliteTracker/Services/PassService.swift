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
    
    func fetchPasses(name: String, line1: String, line2: String, startTime: Date, endTime: Date, gsLat: String, gsLon: String, gsAlt: String, gsMask: String, inputType: InputOptions, searchItem: String) async throws -> [Pass] {
        
        let formatter = ISO8601DateFormatter()
        formatter.timeZone = TimeZone(identifier: "UTC")!
        let startFormatted = formatter.string(from: startTime) // this is translated in UTC time
        let endFormatted = formatter.string(from: endTime)
        
        var req = PassRequest()
        
        if (inputType == .tle) {
            req.line2 = String(searchItem.suffix(69))
            req.line1 = String(searchItem.dropLast(70).suffix(69))
            req.name = String(searchItem.dropLast(139)).trimmingCharacters(in: .whitespaces)
        } else {
            req.line2 = line2
            req.line1 = line1
            req.name = name
        }
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
        //print(String(data: data, encoding: .utf8) ?? "no data")
        return try JSONDecoder().decode([Pass].self, from: data)
    }
}
