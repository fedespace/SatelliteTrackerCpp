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

let launchSites: [String : String] = [
    "AFETR": "Air Force Eastern Test Range, Florida, USA",
    "AFWTR": "Air Force Western Test Range, California, USA",
    "ANDSP": "Andøya Spaceport, Nordland, Norway",
    "ALCLC": "Alâcantara Launch Center, Maranhão, Brazil",
    "BOS":   "Bowen Orbital Spaceport, Queensland, Australia",
    "CAS":   "Canaries Airspace",
    "DLS":   "Dombarovskiy Launch Site, Russia",
    "ERAS":  "Eastern Range Airspace",
    "FRGUI": "Europe's Spaceport, Kourou, French Guiana",
    "HGSTR": "Hammaguira Space Track Range, Algeria",
    "JJSLA": "Jeju Island Sea Launch Area, Republic of Korea",
    "JSC":   "Jiuquan Space Center, PRC",
    "KODAK": "Kodiak Launch Complex, Alaska, USA",
    "KSCUT": "Uchinoura Space Center, Japan",
    "KWAJ":  "US Army Kwajalein Atoll (USAKA)",
    "KYMSC": "Kapustin Yar Missile and Space Complex, Russia",
    "NSC":   "Naro Space Complex, Republic of Korea",
    "PLMSC": "Plesetsk Missile and Space Complex, Russia",
    "RLLB":  "Rocket Lab Launch Base, Mahia Peninsula, New Zealand",
    "SCSLA": "South China Sea Launch Area, PRC",
    "SEAL":  "Sea Launch Platform (mobile)",
    "SEMLS": "Semnan Satellite Launch Site, Iran",
    "SMTS":  "Shahrud Missile Test Site, Iran",
    "SNMLP": "San Marco Launch Platform, Indian Ocean (Kenya)",
    "SPKII": "Space Port Kii, Japan",
    "SRILR": "Satish Dhawan Space Centre, India",
    "SUBL":  "Submarine Launch Platform (mobile)",
    "SVOBO": "Svobodnyy Launch Complex, Russia",
    "TAISC": "Taiyuan Space Center, PRC",
    "TANSC": "Tanegashima Space Center, Japan",
    "TYMSC": "Tyuratam Missile and Space Center, Kazakhstan (Baikonur Cosmodrome)",
    "UNK":   "Unknown",
    "VOSTO": "Vostochny Cosmodrome, Russia",
    "WLPIS": "Wallops Island, Virginia, USA",
    "WOMRA": "Woomera, Australia",
    "WRAS":  "Western Range Airspace",
    "WSC":   "Wenchang Satellite Launch Site, PRC",
    "XICLF": "Xichang Launch Facility, PRC",
    "YAVNE": "Yavne Launch Facility, Israel",
    "YSLA":  "Yellow Sea Launch Area, PRC",
    "YUN":   "Yunsong Launch Site, North Korea"
]
