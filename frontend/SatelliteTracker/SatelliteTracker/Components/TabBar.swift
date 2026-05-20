//
//  TabBar.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 20/05/2026.
//

import SwiftUI

struct TabBar: View {
    var body: some View {
        TabView {
            
            // Home page (2D Map) view
            Tab("", systemImage: "house") {
                    Homepage()
            }
            
            // 3D Map view
            Tab("", systemImage: "globe.europe.africa.fill") {
                    Map3d()
                }
            
            // GS functionalities view
            Tab("", systemImage: "antenna.radiowaves.left.and.right.circle") {
                    GS()
                }
            
            // Setup alerts/notifications view
            Tab("", systemImage: "exclamationmark.magnifyingglass") {
                    Alert()
                }
            
            // Settings view
            Tab("", systemImage: "gear") {
                    Settings()
                }
            
        }
        .tint(.black)
    }
}
