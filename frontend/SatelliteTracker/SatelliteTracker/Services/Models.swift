//
//  Models.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 22/05/2026.
//

import SwiftUI

// Codable: both encodable and decodable (both swift->JSON and JSON->swift)
struct GroundTrackPoint: Codable {
    let name: String
    let norad: String
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

struct SatelliteDetails: Codable {
    var objectName: String
    var objectType: String
    var noradID: Int
    var opsStatusCode: String
    var owner: String
    var launched: String
    var launchSite: String
    var decay: String
    var period: Double
    var incl: Double
    var apogee: Int
    var perigee: Int
    var rcs: Double
    var dataStatusCode: String
    var orbitCenter: String
    var orbitType: String
    
    enum CodingKeys: String, CodingKey {
        case objectName = "OBJECT_NAME"
        case objectType = "OBJECT_TYPE"
        case noradID = "NORAD_CAT_ID"
        case opsStatusCode = "OPS_STATUS_CODE"
        case owner = "OWNER"
        case launched = "LAUNCH_DATE"
        case launchSite = "LAUNCH_SITE"
        case decay = "DECAY_DATE"
        case period = "PERIOD"
        case incl = "INCLINATION"
        case apogee = "APOGEE"
        case perigee = "PERIGEE"
        case rcs = "RCS"
        case dataStatusCode = "DATA_STATUS_CODE"
        case orbitCenter = "ORBIT_CENTER"
        case orbitType = "ORBIT_TYPE"
    }
}

extension SatelliteDetails {
    static let empty = SatelliteDetails(
        objectName: "",
        objectType: "",
        noradID: 0,
        opsStatusCode: "",
        owner: "",
        launched: "",
        launchSite: "",
        decay: "",
        period: 0,
        incl: 0,
        apogee: 0,
        perigee: 0,
        rcs: 0.0,
        dataStatusCode: "",
        orbitCenter: "",
        orbitType: ""
    )
}
