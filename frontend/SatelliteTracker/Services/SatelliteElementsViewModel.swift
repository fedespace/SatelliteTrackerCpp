//
//  SatelliteElementsViewModel.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 22/06/2026.
//

import SwiftUI

@Observable
class SatelliteElementsViewModel {
    var errorMessage: String? = nil
    var elements: String = ""

    func fetchElements() async -> String {
        do {
            elements = try await SatelliteElementsService().fetchElements()
        } catch {
            print("Error: \(error)")
            errorMessage = error.localizedDescription
        }
        return elements
    }
}
