//
//  FavouritesStore.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 08/07/2026.
//

import SwiftUI
import Combine

class FavouritesStore: ObservableObject {
    @Published var savedInputs: [SavedSatellites] = []
    private let backend: FavouritesStoring
    
    init(backend: FavouritesStoring = UserDefaultFavouritesStore()) {
        self.backend = backend
        self.savedInputs = SavedSatellites.defaults + backend.load()
    }
    
    func add(_ satellite: SavedSatellites) {
        savedInputs.append(satellite)
        let _ = savedInputs.filter { sat in
            !SavedSatellites.defaults.contains(where: {$0.id == sat.id})
        }
        backend.save(savedInputs)
    }
    
    func remove(searchItem: String, input: InputOptions) {
        let normalizedText = searchItem.lowercased().trimmingCharacters(in: .whitespacesAndNewlines)
        let savedNames = Set(savedInputs.map{ $0.name?.lowercased() })
        let savedNORAD = Set(savedInputs.map{ $0.norad })
        if (input == .name) {
            if savedNames.contains(normalizedText) {
                savedInputs.removeAll { satellite in
                    satellite.name?.lowercased().trimmingCharacters(in: .whitespacesAndNewlines) == normalizedText
                }
            }
        } else {
            if savedNORAD.contains(normalizedText) {
                savedInputs.removeAll { satellite in
                    satellite.norad?.trimmingCharacters(in: .whitespacesAndNewlines) == normalizedText
                }
            }
        }
    }
}
