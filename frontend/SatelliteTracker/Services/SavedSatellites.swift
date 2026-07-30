//
//  SavedSatellites.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 08/07/2026.
//

import SwiftUI

struct SavedSatellites: Codable, Identifiable, Hashable {
    let id: UUID
    let name: String?
    let norad: String?
}

protocol FavouritesStoring {
    func load() -> [SavedSatellites]
    func save(_ satellites: [SavedSatellites])
}

extension SavedSatellites {
    static let defaults: [SavedSatellites] = [
        SavedSatellites(id: UUID(), name: "ISS (ZARYA)", norad: "25544"),
        SavedSatellites(id: UUID(), name: "HST", norad: "20580")
    ]
}
