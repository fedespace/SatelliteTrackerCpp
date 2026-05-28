//
//  GroundTrackViewModel.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 22/05/2026.
//

import SwiftUI
import MapKit

@Observable
class GroundTrackViewModel {
    var points: [String: GroundTrackPoint] = [:]
    var isLoading: Bool = false
    var errorMessage: String? = nil
    // Computed property for the coordinates
    var coordinates: [CLLocationCoordinate2D] {
        points.sorted { $0.key < $1.key }
              .map { CLLocationCoordinate2D(latitude: $0.value.lat, longitude: $0.value.lon) }
    }
    
    func fetchGroundTrack(inputType: InputOptions, searchItem: String, start: Date, end: Date, step: String) async {
        isLoading = true
        do {
            points = try await GroundTrackService().fetchGroundTrack(inputType: inputType, searchItem: searchItem, start: start, end: end, step: step)
            print(points)
        } catch {
            errorMessage = error.localizedDescription
        }
        isLoading = false
    }
}
