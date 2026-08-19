//
//  TabBar.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 20/05/2026.
//

import SwiftUI

struct TabBar: View {
    
    @State private var sat: String = ""
    @State private var startTime = Date()
    @State private var endTime = Date()
    @State private var inputType: InputOptions = .name
    @State private var searchItem: String?
    @State private var gtViewModel = GroundTrackViewModel()
    
    var body: some View {
        
        TabView {
            
            // Home page (2D Map) view
            Tab("", systemImage: "house") {
                Homepage(inputType: $inputType, searchItem: $searchItem, gtViewModel: $gtViewModel, startTime: $startTime, endTime: $endTime, sat: $sat)
            }
            
            // 3D Map view
            Tab("", systemImage: "globe.europe.africa.fill") {
                    Map3d()
                }
            
            // GS functionalities view
            Tab("", systemImage: "antenna.radiowaves.left.and.right.circle") {
                    GS(satellite: $sat, startTime: $startTime, endTime: $endTime, inputType: $inputType, searchItem: $searchItem, gtViewModel: $gtViewModel)
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
        .background(.black)
        .tint(.black)
    }
}

#Preview {
    TabBar()
}
