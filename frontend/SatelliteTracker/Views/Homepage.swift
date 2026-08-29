//
//  Homepage.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 20/05/2026.
//

import SwiftUI
import MapKit

// Homepage structure
struct Homepage: View {
    
    @Binding var inputType: InputOptions
    @Binding var searchItem: String?
    @Binding var gtViewModel: GroundTrackViewModel
    @Binding var startTime: Date
    @Binding var endTime: Date
    @State private var showEndTime = false
    @State private var step: String = ""
    @Binding var showDetails: Bool
    @State private var mapC: MapCameraPosition = .camera(MapCamera(
        centerCoordinate: CLLocationCoordinate2D(latitude: 0, longitude: 0),
        distance: 200_000_000,
        heading: 0.0,
        pitch: 0.0
    ))
    @State private var detailsViewModel = SatelliteDetailsViewModel()
    var noradID: String { gtViewModel.norad }
    @Binding var detailsSatellite: SatelliteDetails
    @State private var isDetailVisible: Bool = false
    var opsStatus: String {detailsSatellite.opsStatusCode}
    var owner: String {detailsSatellite.owner}
    var launchDate: String {detailsSatellite.launched}
    var launchSite: String {detailsSatellite.launchSite}
    var periodOrbit: Double {detailsSatellite.period}
    var inclinationOrbit: Double {detailsSatellite.incl}
    var apogee: Int {detailsSatellite.apogee}
    var perigee: Int {detailsSatellite.perigee}
    @State private var elementsViewModel = SatelliteElementsViewModel()
    @State private var elementsSatellite: String = ""
    @StateObject private var favouritesStore = FavouritesStore()
    var a: Double {Double((apogee + perigee)/2 + 6371)}
    @State private var selectedCoordinate = CLLocationCoordinate2D()
    @FocusState private var isTextFocused: Bool
    @Binding var sat: String
    
    
    var body: some View {
            
        ZStack {
            
            Color.yaleBlue.ignoresSafeArea()
            
            ScrollView {
                // VSTACK #1 - TITLE + INPUTS + EARTH MAP ==================================================================================
                VStack(spacing: 5) {
                    
                    // TITLE ===============================================================================================================
                    Text("SkyLens")
                        .foregroundStyle(Color.ivoryMist)
                        .font(.mainTitleFont)
                        .frame(maxWidth: .infinity, alignment: .leading)
                    
                    
                    // VSTACK #2 - INPUT PICKER + GRID (START / END? PICKER ROWS) + TEXTFIELD? ======================
                    VStack(alignment: .leading, spacing: 5) {
                        
                        // HSTACK #1 - INPUT TYPE PICKER + ARROW BUTTON ====================================================================
                        
                        InputPicker(inputType: $inputType, searchItem: $searchItem, detailsSatellite: $detailsSatellite, endTime: $endTime, startTime: $startTime, showDetails: $showDetails, showEndTime: $showEndTime, gtViewModel: $gtViewModel, step: $step, favouritesStore: favouritesStore)
                            .onSubmit {
                                Task {
                                    detailsSatellite = SatelliteDetails.empty
                                    endTime = (showEndTime) ? endTime : startTime.addingTimeInterval(1.0)
                                    if (endTime > startTime) {
                                        await gtViewModel.fetchGroundTrack(inputType: inputType, searchItem: searchItem!.uppercased(), start: startTime, end: endTime, step: step)
                                        sat = noradID
                                    }
                                    
                                }
                                showDetails = true
                            }
                            .zIndex(100.0)

                        GlassEffectContainer {
                            TextField(
                                 "",
                                 text: Binding(
                                    get: { searchItem ?? "" },
                                    set: { searchItem = $0.isEmpty ? nil : $0 } // Keeps it nil if empty, or updates it
                                ),
                                 prompt: Text("Search by \(inputType.rawValue)...".uppercased()).kerning(2).foregroundStyle(.white.opacity(0.7))
                             )
                             .focused($isTextFocused)
                             .foregroundStyle(.white)
                             .padding(.leading, 10)
                             .padding(.vertical, 5)
                             .frame(maxWidth: .infinity, minHeight: 25, alignment: .leading)
                             .font(.inputPickerFont)
                             .glassEffect(.regular)
                             .onSubmit {
                                 Task {
                                     endTime = (showEndTime) ? endTime : startTime.addingTimeInterval(1.0)
                                     if (endTime > startTime) {
                                         await gtViewModel.fetchGroundTrack(inputType: inputType, searchItem: searchItem!.uppercased(), start: startTime, end: endTime, step: step)
                                         sat = noradID
                                     }
                                 }
                                 showDetails = true
                             }
                            
                        }
                        
                       
                        
                        HStack(alignment: .center) {
                                
                            // Label for start picker
                            Image(systemName: "mappin.and.ellipse")
                                .font(.system(size: 16))
                                .foregroundStyle(Color.ivoryMist)
                                .opacity(0.85)
                                
                            
                            // Start date and time picker (custom with overlay)
                            GlassEffectContainer {
                                RoundedRectangle(cornerRadius: 50)
                                        .fill(Color.clear)
                                        .overlay(
                                            HStack (spacing: 10) {
                                                Image(systemName: "calendar.badge.clock").opacity(0.65)
                                                Text(startTime.formatted(.dateTime.day().month().year()))
                                                    .foregroundStyle(.opacity(0.65))
                                                    .font(.system(size: 19))
                                                Text(startTime.formatted(.dateTime.hour().minute()))
                                                    .foregroundStyle(.opacity(0.65))
                                                    .font(.system(size: 19))
                                            }
                                                .padding(.horizontal, 10)
                                                .frame(maxWidth: .infinity)
                                            
                                        )
                                .padding(.vertical, 10)
                                .frame(width: 220, height: 30)
                                .glassEffect(.regular.tint(.ivoryMist.opacity(0.1)))
                            }
                            .overlay {
                                DatePicker("", selection: $startTime, displayedComponents: [.date, .hourAndMinute])
                                    
                                    .colorMultiply(Color.clear)
                                    .accentColor(Color.darkSlateGrey)
                                    .font(.system(size: 20))
                                    .onChange(of: startTime) {
                                        Task {
                                            endTime = (showEndTime) ? endTime : startTime.addingTimeInterval(1.0)
                                            if (endTime > startTime) {
                                                await gtViewModel.fetchGroundTrack(inputType: inputType, searchItem: searchItem!.uppercased(), start: startTime, end: endTime, step: step)
                                                sat = noradID
                                            }
                                        }
                                        showDetails = true
                                    }
                            }
                            
                            
                            // Toggle for end time picker
                            Image("groundtrack")
                                .foregroundStyle(Color.ivoryMist)
                                .font(.system(size: 16))
                                .opacity(0.85)
                                .scaleEffect(0.85)
                                .padding(.leading, 45)
                                .padding(.trailing, -5)
                            
                            RoundedRectangle(cornerRadius: 5)
                                .fill(showEndTime ? Color.darkSlateGrey.opacity(0.7) : Color.clear)
                                .stroke(showEndTime ? Color.gray : Color.gray.opacity(0.4), lineWidth: 1)
                                .overlay(showEndTime ? Image(systemName: "checkmark").foregroundStyle(Color.ivoryMist.opacity(0.8)) : nil)
                                .frame(width:20, height: 20)
                                .onTapGesture {
                                    withAnimation(.easeInOut(duration: 0.3)) {
                                        showEndTime.toggle()
                                        // one sec difference from start to be able to kick off propagator
                                        endTime = (showEndTime) ? startTime.addingTimeInterval(60.0) : startTime.addingTimeInterval(1.0)
                                        Task {
                                            if (endTime > startTime) {
                                                await gtViewModel.fetchGroundTrack(inputType: inputType, searchItem: searchItem!.uppercased(), start: startTime, end: endTime, step: step)
                                                sat = noradID
                                            }
                                        }
                                        showDetails = true
                                    }
                                }
                        }
                        .padding(.leading, 10)
                        .padding(.top, 7)
                        .frame(maxWidth: .infinity, alignment: .leading)
                        
                        
                            
                        // GRID ROW in case "tracks" option is selected ==================================================================
                        if showEndTime {
                            HStack(alignment: .center) {
                                
                                // Label for end picker
                                Image(systemName: "pin.fill")
                                    .foregroundStyle(Color.ivoryMist.opacity(0.85))
                                    .font(.system(size: 16))
                                    .scaleEffect(0.85)
                                    
                                
                                // End date and time picker
                                GlassEffectContainer {
                                    RoundedRectangle(cornerRadius: 50)
                                            .fill(Color.clear)
                                            .overlay(
                                                HStack (spacing: 10) {
                                                    Image(systemName: "calendar.badge.clock").opacity(0.65)
                                                    Text(endTime.formatted(.dateTime.day().month().year()))
                                                        .foregroundStyle(.opacity(0.65))
                                                        .font(.system(size: 19))
                                                    Text(endTime.formatted(.dateTime.hour().minute()))
                                                        .foregroundStyle(.opacity(0.65))
                                                        .font(.system(size: 19))
                                                }
                                                    .padding(.horizontal, 10)
                                                    .frame(maxWidth: .infinity)
                                                
                                            )
                                    .padding(.vertical, 10)
                                    .frame(width: 220, height: 30)
                                    .glassEffect(.regular.tint(.ivoryMist.opacity(0.1)))
                                }
                                .overlay {
                                    DatePicker("", selection: $endTime, displayedComponents: [.date, .hourAndMinute])
                                        .colorMultiply(Color.clear)
                                        .accentColor(Color.darkSlateGrey)
                                        .onChange(of: endTime) {
                                            Task {
                                                if (endTime > startTime) {
                                                    await gtViewModel.fetchGroundTrack(inputType: inputType, searchItem: searchItem!.uppercased(), start: startTime, end: endTime, step: step)
                                                    sat = noradID
                                                }
                                            }
                                            showDetails = true
                                        }
                                }
                                
                                Image(systemName: "clock.badge")
                                    .foregroundStyle(Color.ivoryMist)
                                    .font(.system(size: 16))
                                    .opacity(0.85)
                                    .padding(.leading, 30)
                                    
                                
                                // Step selection (interval between two computations)
                                RoundedRectangle(cornerRadius: 5)
                                    .fill(Color.darkSlateGrey.opacity(0.7))
                                    .stroke(Color.gray)
                                    .overlay(TextField("", text: $step).keyboardType(.numberPad)
                                        .foregroundStyle(Color.gray)
                                        .font(Font.tracksStep)
                                        .padding(8)
                                        .onSubmit {
                                            Task {
                                                if (endTime > startTime) {
                                                    await gtViewModel.fetchGroundTrack(inputType: inputType, searchItem: searchItem!.uppercased(), start: startTime, end: endTime, step: step)
                                                    sat = noradID
                                                }
                                            }
                                            showDetails = true
                                        })
                                    .frame(width: 35, height: 20)
                            }
                            .padding(.leading, 10)
                            .frame(maxWidth: .infinity, alignment: .leading)
                            
                        }
                        
                    }
                    
                    
                    // View of the 2D Map
                    
                    MapReader { reader in
                        
                        Map (position: $mapC) {
                            
                            MapPolyline(coordinates: gtViewModel.coordinates)
                                .stroke(Color.darkSlateGrey, lineWidth: 2)
                            
                            
                            ForEach(gtViewModel.coordinates, id: \.latitude) { point in
                                let c = CLLocationCoordinate2D(latitude: point.latitude, longitude: point.longitude)
                                Annotation("", coordinate: c) {
                                    Circle().fill(Color.burntSienna)
                                        .frame(width: 5, height: 5)
                                }
                            }
                        }
                        .onChange(of: gtViewModel.coordinates.first?.latitude, {
                            mapC = gtViewModel.coordinates.first?.latitude != nil ?
                                .camera(MapCamera(
                                    centerCoordinate: gtViewModel.coordinates.first!,
                                    distance: 200_000_000,
                                    heading: 0.0,
                                    pitch: 0.0
                                ))
                            : .camera(MapCamera(
                                centerCoordinate: CLLocationCoordinate2D(latitude: 0, longitude: 0),
                                distance: 200_000_000,
                                heading: 0.0,
                                pitch: 0.0
                            ))
                        })
                        .clipShape(RoundedRectangle(cornerRadius: 20))
                        .frame(minHeight: 300, maxHeight: 300)
                        .mapStyle(.standard(elevation: .flat))
                        .padding(.top, 15)
                        .padding(.bottom,15)
                    }
                    
                    if(showDetails) {
                        withAnimation(.easeInOut(duration: 0.3)) {
                            GlassEffectContainer {
                                VStack { // the entire bubble for all details
                                    Text("\(gtViewModel.satName) • \(gtViewModel.norad)")
                                        .font(Font.detailTitle).foregroundStyle(Color.darkSlateGrey)
                                        .frame(maxWidth: .infinity, alignment: .center)
                                        .padding(.bottom, 15)
                                    
                                    VStack (alignment: .leading) {
                                        
                                        if (opsStatus == "-" || opsStatus == "D") {
                                            HStack {
                                                Text("Not Operational")
                                                    .font(Font.detailBody).foregroundStyle(Color.darkSlateGrey)
                                                Image(systemName: "x.circle")
                                                    .foregroundStyle(Color.darkSlateGrey)
                                            }
                                            .task {
                                                elementsSatellite = await elementsViewModel.fetchElements()
                                            }
                                        } else {
                                            HStack {
                                                Text("Operational")
                                                    .font(Font.detailBody).foregroundStyle(Color.darkSlateGrey)
                                                Image(systemName: "checkmark.circle")
                                                    .foregroundStyle(Color.darkSlateGrey)
                                            }
                                        }
                                        
                                        
                                        
                                        
                                        Text("Owner: \(owner)")
                                            .font(Font.detailBody).foregroundStyle(Color.darkSlateGrey)
                                        Text("Launched on: \(launchDate)")
                                            .font(Font.detailBody).foregroundStyle(Color.darkSlateGrey)
                                        Text("From: \(launchSites[launchSite] ?? launchSite)")
                                            .font(Font.detailBody).foregroundStyle(Color.darkSlateGrey)
                                        
                                    }
                                        
                                    VStack (alignment: .trailing) {
                                        Text("Orbital Elements").font(Font.subtitleDetail)
                                        Text("Period: \(periodOrbit.formatted(.number.precision(.fractionLength(2)))) min")
                                        Text("Semi-major Axis: \(a.formatted(.number.precision(.fractionLength(2)))) km")
                                        Text("Inclination: \(inclinationOrbit.formatted(.number.precision(.fractionLength(2)))) degrees")
                                        Text("Apogee Altitude: \(apogee.formatted(.number.precision(.fractionLength(2)))) km")
                                        Text("Perigee Altitude: \(perigee.formatted(.number.precision(.fractionLength(2)))) km")
                                    }
                                    .padding(.top, 15)
                                    .frame(maxWidth: .infinity)
                                    .font(Font.detailBody).foregroundStyle(Color.darkSlateGrey)
                                }
                                .padding(.vertical, 30)
                                .frame(maxWidth: .infinity, minHeight: 150.0)
                                .opacity(isDetailVisible ? 1 : 0)
                                .onChange(of: noradID, { oldValue, newValue in
                                    Task {
                                        detailsSatellite = await detailsViewModel.fetchDetails(norad: newValue)
                                        withAnimation {
                                            isDetailVisible = true
                                        }
                                    }
                                })
                            }
                            .glassEffect(.regular, in: .rect(cornerRadius: 20))
                        
                        }
                        
                    }
                    
                    Spacer()
                }
                .padding([.leading, .trailing], 30)
                .ignoresSafeArea()
                
            }
            
        }
        .onTapGesture {
            isTextFocused = false
        }
        .ignoresSafeArea(.keyboard, edges: .bottom)
        
        
        
    }
    
}
   
