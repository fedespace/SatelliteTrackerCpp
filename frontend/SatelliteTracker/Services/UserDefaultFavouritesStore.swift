//
//  UserDefaultFavouritesStore.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 08/07/2026.
//

import SwiftUI

class UserDefaultFavouritesStore: FavouritesStoring {
    private let key = "savedSatellites" // in the "dictionary" UserDefault we are saving this data under this key
    
    // UserDefaults.standard.removeObject(forKey: key)
    func load() -> [SavedSatellites] {
        UserDefaults.standard.removeObject(forKey: key)
        guard let data = UserDefaults.standard.data(forKey: key),
              let decoded = try? JSONDecoder().decode([SavedSatellites].self, from: data)
        else {return []}
        
        // Get a list of strings of the initial SavedSatellite (the default ones) and get only unique values
        let defaultKeys = Set(SavedSatellites.defaults.map{ $0.name })
        // using filter: checks if decoded array contains one of the names of the already saved satellite as defaults and if so it doesn't keep it
        let unique = decoded.filter { !defaultKeys.contains($0.name) }
        
        return unique
    }
    
    func save(_ satellites: [SavedSatellites]) {
        if let encoded = try? JSONEncoder().encode(satellites) {
            //print(encoded)
            UserDefaults.standard.set(encoded, forKey: key)
        }
    }
}
