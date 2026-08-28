//
//  TabBar.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 20/05/2026.
//

import SwiftUI

enum SelectedTab: String, CaseIterable {
    case map2d
    case map3d
    case gs
    case alerts
    case settings
}

struct TabBar: View {
    
    @State private var selectedTab: SelectedTab = .map2d
    @State private var sat: String = ""
    @State private var startTime = Date()
    @State private var endTime = Date()
    @State private var inputType: InputOptions = .name
    @State private var searchItem: String?
    @State private var gtViewModel = GroundTrackViewModel()
    @State private var showDetails: Bool = false
    
    var body: some View {
        
        ZStack (alignment: .bottom) {
            
            // Assign the number to the tab
            switch(selectedTab) {
            case .map2d:
                Homepage(inputType: $inputType, searchItem: $searchItem, gtViewModel: $gtViewModel, startTime: $startTime, endTime: $endTime, showDetails: $showDetails, sat: $sat)
            case .map3d:
                Map3d()
            case .gs:
                GS(satellite: $sat, startTime: $startTime, endTime: $endTime, inputType: $inputType, searchItem: $searchItem, gtViewModel: $gtViewModel)
            case .alerts:
                Alert()
            case .settings:
                Settings()
            }
            
            HStack (spacing: 32) {
                Button(action: { selectedTab = .map2d }) {
                    Image("map2d")
                        .resizable()
                        .frame(width: 30, height: 30)
                        .foregroundStyle(showDetails ? Color.prussianBlue : Color.ivoryMist)
                        .opacity(selectedTab == .map2d ? 1 : 0.5)
                }
                Button(action: {
                    selectedTab = .map3d
                    showDetails = false
                }) {
                    Image("map3d")
                        .resizable()
                        .frame(width: 30, height: 30)
                        .foregroundStyle(showDetails ? Color.prussianBlue : Color.ivoryMist)
                        .opacity(selectedTab == .map3d ? 1 : 0.5)
                }
                Button(action: {
                    selectedTab = .gs
                    showDetails = false
                }) {
                    Image("gsmenu")
                        .resizable()
                        .frame(width: 30, height: 30)
                        .foregroundStyle(showDetails ? Color.prussianBlue : Color.ivoryMist)
                        .opacity(selectedTab == .gs ? 1 : 0.5)
                }
                Button(action: {
                    selectedTab = .alerts
                    showDetails = false
                }) {
                    Image("alerts")                        .resizable()
                        .frame(width: 30, height: 30)
                        .foregroundStyle(showDetails ? Color.prussianBlue : Color.ivoryMist)
                        .opacity(selectedTab == .alerts ? 1 : 0.5)
                }
                Button(action: {
                    selectedTab = .settings
                    showDetails = false
                }) {
                    Image("settings")
                        .resizable()
                        .frame(width: 30, height: 30)
                        .foregroundStyle(showDetails ? Color.prussianBlue : Color.ivoryMist)
                        .opacity(selectedTab == .settings ? 1 : 0.5)
                }
            }
            .frame(width: 350, height: 55, alignment: .center)
            .glassEffect(.regular, in: .capsule)
        }
    }
}


#Preview {
    TabBar()
}
