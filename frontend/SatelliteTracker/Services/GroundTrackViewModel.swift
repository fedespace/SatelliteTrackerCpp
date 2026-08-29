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
    var tleLine1: String {
        points.values.first?.line1 ?? ""
    }
    var tleLine2: String {
        points.values.first?.line2 ?? ""
    }
    var coord3dmap: [SIMD3<Float>] {
        func latLonAltToSCNVector3(lat: Double, lon: Double, alt: Double, radius: Double) -> SIMD3<Float> {
            let deg2rad = Double.pi / 180
            let altitude_scaled: Double = 0.000070 * alt
            let l = -lat*deg2rad
            let ll = lon*deg2rad
            let R = radius + altitude_scaled
            let x: Float = Float(-R * cos(l) * cos(ll))
            let y: Float = Float(-R * sin(l))
            let z: Float = Float(R * cos(l) * sin(ll))
            return SIMD3(x: x, y: y, z: z)
        }
        return points.sorted { $0.key < $1.key }
            .map { latLonAltToSCNVector3(lat: $0.value.lat, lon: $0.value.lon, alt: 1200.0, radius: 0.45)
            }
    }
    
    var satName: String = ""
    var norad: String = ""
    
    func fetchGroundTrack(inputType: InputOptions, searchItem: String, start: Date, end: Date, step: String) async {
        isLoading = true
        do {
            points = try await GroundTrackService().fetchGroundTrack(inputType: inputType, searchItem: searchItem, start: start, end: end, step: step)
            satName = points.first?.value.name ?? ""
            norad = points.first?.value.norad ?? ""
        } catch {
            errorMessage = error.localizedDescription
        }
        isLoading = false
    }
}
