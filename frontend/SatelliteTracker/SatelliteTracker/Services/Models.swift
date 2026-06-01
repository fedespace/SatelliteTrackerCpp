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
    var name: String = ""
    var line1: String = ""
    var line2: String = ""
    var startTime: String = ""
    var endTime: String = ""
    var stepInterval: String = ""
}

struct NameRequest: Codable {
    var name: String = ""
    var startTime: String = ""
    var endTime: String = ""
    var stepInterval: String = ""
}

struct NoradRequest: Codable {
    var norad: String = ""
    var startTime: String = ""
    var endTime: String = ""
    var stepInterval: String = ""
}

struct ISS_HUBBLE: Codable {
    var satellite: String = ""
    var startTime: String = ""
    var endTime: String = ""
    var stepInterval: String = ""
}
