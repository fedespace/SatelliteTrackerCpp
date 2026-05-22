//
//  Models.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 22/05/2026.
//

import SwiftUI

// Codable: both encodable and decodable (both swift->JSON and JSON->swift)
struct GroundTrackPoint: Codable {
    let lat: Double
    let lon: Double
}

struct TleRequest: Codable {
    let name: String
    let line1: String
    let line2: String
}
