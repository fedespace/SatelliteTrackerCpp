//
//  TabBar.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 20/05/2026.
//

import SwiftUI

enum SelectedTab: String, CaseIterable {
    case menu_map2d
    case menu_map3d
    case menu_gs
    case menu_alerts
    case menu_settings
}

struct TabBar: View {
    
    @State private var selectedTab: SelectedTab = .menu_map2d
    @State private var menuItemsList = SelectedTab.allCases
    @State private var sat: String = ""
    @State private var startTime = Date()
    @State private var endTime = Date()
    @State private var inputType: InputOptions = .name
    @State private var searchItem: String?
    @State private var gtViewModel = GroundTrackViewModel()
    @State private var showDetails: Bool = false
    @State private var offsetBubble: CGFloat = -120.0
    @State private var detailsSatellite: SatelliteDetails = .empty
    
    var body: some View {
        
        ZStack (alignment: .bottom) {
            
            // Assign the number to the tab
            switch(selectedTab) {
            case .menu_map2d:
                Homepage(inputType: $inputType, searchItem: $searchItem, gtViewModel: $gtViewModel, startTime: $startTime, endTime: $endTime, showDetails: $showDetails, detailsSatellite: $detailsSatellite, sat: $sat)
            case .menu_map3d:
                Map3d(gtViewModel: $gtViewModel, detailsSatellite: $detailsSatellite, coord3dmap: .constant(gtViewModel.coord3dmap), startTime: $startTime, endTime: $endTime)
            case .menu_gs:
                GS(satellite: $sat, startTime: $startTime, endTime: $endTime, inputType: $inputType, searchItem: $searchItem, gtViewModel: $gtViewModel)
            case .menu_alerts:
                Alert()
            case .menu_settings:
                Settings()
            }
            
            
            
            ZStack {
                RoundedRectangle(cornerRadius: 30)
                    .frame(width: 315, height: 55)
                    .glassEffect(.regular)
                
                RoundedRectangle(cornerRadius: 30)
                    .fill(Color.ivoryMist.opacity(0.2))
                    .frame(width: 60, height: 45)
                    .offset(x: offsetBubble)
                    .animation(.easeOut(duration: 0.5), value: offsetBubble)
                    
                HStack (spacing: 30) {
                    ForEach ($menuItemsList, id: \.self) { opt in
                        Button(action: {
                            selectedTab = opt.wrappedValue
                            offsetBubble = CGFloat((SelectedTab.allCases.firstIndex(of: opt.wrappedValue)! - 2) * 60)
                        }) {
                            Image(opt.wrappedValue.rawValue)
                                .resizable()
                                .frame(width: 30, height: 30)
                                .foregroundStyle((selectedTab == .menu_map2d && showDetails) ? Color.prussianBlue : Color.ivoryMist)
                                .opacity(selectedTab == opt.wrappedValue ? 1 : 0.7)
                        }
                    }
                }
                .frame(width: 300)
                .zIndex(100.0)
                
            }
            .offset(y: 15)
            .frame(width: 300)
            
            
            
            //            .frame(width: 350, height: 55, alignment: .center)
//            .glassEffect(.clear, in: .capsule)
            
//            RoundedRectangle(cornerRadius: 30)
//                .fill(Color.ivoryMist)
//                .opacity(selectedTab == opt.wrappedValue ? 0.3 : 0)
//                .frame(width: 70, height: 50)
            
        }
    }
}
            
//            HStack (spacing: 32) {
//                Button(action: { selectedTab = .map2d }) {
//                    RoundedRectangle(cornerRadius: 30)
//                        .fill(Color.ivoryMist)
//                        .opacity(0.3)
//                        .frame(width: 70, height: 50)
//                        .overlay {
//                            Image("map2d")
//                                .resizable()
//                                .frame(width: 30, height: 30)
//                                .foregroundStyle(showDetails ? Color.prussianBlue : Color.ivoryMist)
//                                .opacity(selectedTab == .map2d ? 1 : 0.5)
//                        }
//                }
//                Button(action: {
//                    selectedTab = .map3d
//                    showDetails = false
//                }) {
//                    Image("map3d")
//                        .resizable()
//                        .frame(width: 30, height: 30)
//                        .foregroundStyle(showDetails ? Color.prussianBlue : Color.ivoryMist)
//                        .opacity(selectedTab == .map3d ? 1 : 0.5)
//                }
//                Button(action: {
//                    selectedTab = .gs
//                    showDetails = false
//                }) {
//                    Image("gsmenu")
//                        .resizable()
//                        .frame(width: 30, height: 30)
//                        .foregroundStyle(showDetails ? Color.prussianBlue : Color.ivoryMist)
//                        .opacity(selectedTab == .gs ? 1 : 0.5)
//                }
//                Button(action: {
//                    selectedTab = .alerts
//                    showDetails = false
//                }) {
//                    Image("alerts")                        .resizable()
//                        .frame(width: 30, height: 30)
//                        .foregroundStyle(showDetails ? Color.prussianBlue : Color.ivoryMist)
//                        .opacity(selectedTab == .alerts ? 1 : 0.5)
//                }
//                Button(action: {
//                    selectedTab = .settings
//                    showDetails = false
//                }) {
//                    Image("settings")
//                        .resizable()
//                        .frame(width: 30, height: 30)
//                        .foregroundStyle(showDetails ? Color.prussianBlue : Color.ivoryMist)
//                        .opacity(selectedTab == .settings ? 1 : 0.5)
//                }
//            }
//            .frame(width: 350, height: 55, alignment: .center)
//            .glassEffect(.regular, in: .capsule)


#Preview {
    TabBar()
}
