//
//  SatelliteDetailsViewModel.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 20/06/2026.
//

import SwiftUI

@Observable
class SatelliteDetailsViewModel {
    var errorMessage: String? = nil
    var details = SatelliteDetails.empty

    func fetchDetails(norad: String) async -> SatelliteDetails {
        do {
            details = try await SatelliteDetailsService().fetchDetails(norad: norad)
        } catch {
            print("Error: \(error)")
            errorMessage = error.localizedDescription
        }
        return details
    }
}
