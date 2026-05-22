//
//  GroundTrackService.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 22/05/2026.
//

import SwiftUI

struct GroundTrackService {
    
    func fetchGroundTrack(tle: TleRequest) async throws -> [String: GroundTrackPoint] {
        
        // Request:
        var request = URLRequest(url: URL(string: "http://localhost:8080/groundtrack")!)
        request.httpMethod = "POST"
        request.setValue("application/json", forHTTPHeaderField: "Content-Type")
        request.httpBody = try JSONEncoder().encode(tle) // from swift to JSON and then to server
        
        let (data, _) = try await URLSession.shared.data(for:request)
        return try JSONDecoder().decode([String: GroundTrackPoint].self, from: data)
    }
    
}
