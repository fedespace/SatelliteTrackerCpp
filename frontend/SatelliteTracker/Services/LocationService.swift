//
//  LocationService.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 31/08/2026.
//

import SwiftUI
import CoreLocation
import Combine


class LocationService: NSObject, ObservableObject {
    @Published var latitude: Double?
    @Published var longitude: Double?
    @Published var altitude: Double?
    @State private var location: CLLocation?
    var error = ""
    @Published var authorizationStatus: CLAuthorizationStatus = .notDetermined
    
    func requestLocation() {
        Task {
            if authorizationStatus == .notDetermined {
                CLLocationManager().requestWhenInUseAuthorization()
            } else if (authorizationStatus == .denied || authorizationStatus == .restricted) {
                error = "Authorization is denied."
            } else {
                for try await update in CLLocationUpdate.liveUpdates() {
                    location = update.location
                    latitude = location?.coordinate.latitude
                    longitude = location?.coordinate.longitude
                    altitude = location?.altitude
            }
            
                
            }
        }
    }
}
