//
//  FavoritesStore.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 08/07/2026.
//

import SwiftUI

class FavouritesStore: ObservableObject {
    @Published var savedInputs: [SavedSatellites] = []
    private let backend: FavouritesStoring
}
