//
//  SatelliteDetailsService.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 20/06/2026.
//

import SwiftUI

struct SatelliteDetailsService {
    
    func fetchDetails(norad: String) async throws -> SatelliteDetails {
        let request = URLRequest(url: URL(string: "https://celestrak.org/satcat/records.php?CATNR=\(norad)&FORMAT=JSON-PRETTY")!)
        let (data, _) = try await URLSession.shared.data(for:request)
        print(String(data: data, encoding: .utf8) ?? "nil")
        return try JSONDecoder().decode([SatelliteDetails].self, from: data).first!
    }
    
}
