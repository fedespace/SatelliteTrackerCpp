//
//  GroundTrackViewModel.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 22/05/2026.
//

import SwiftUI

@Observable
class GroundTrackViewModel {
    var points: [String: GroundTrackPoint] = [:]
    var isLoading: Bool = false
    var errorMessage: String? = nil
    
    func fetchGroundTrack(tle: TleRequest) async {
        isLoading = true
        do {
            points = try await GroundTrackService().fetchGroundTrack(tle: tle)
        } catch {
            errorMessage = error.localizedDescription
        }
        isLoading = false
    }
}
