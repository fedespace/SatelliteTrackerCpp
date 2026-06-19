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
    
    @State private var inputType: InputOptions = .tle
    @State private var searchItem: String = ""
    @State private var gtViewModel = GroundTrackViewModel()
    @State private var mapRegion: MKCoordinateRegion = MKCoordinateRegion(
        center: CLLocationCoordinate2D(latitude: 0, longitude: 0),
        span: MKCoordinateSpan(latitudeDelta: 180, longitudeDelta: 360))
    @State private var mapCamera: MapCameraPosition = .automatic
    @State private var startTime = Date()
    @State private var endTime = Date()
    @State private var showEndTime = false
    @State private var step: String = ""
    @State private var showTextfield: Bool = true
    @State private var showDetails: Bool = false
    @State private var updateCenterMap: Bool = false
    
    var body: some View {
        
        // VSTACK #1 - TITLE + INPUTS + EARTH MAP ==================================================================================
        VStack(spacing: 10) {
            
            // TITLE ===============================================================================================================
            Text("SkyLens")
                .font(.mainTitleFont)
                .kerning(7)
                .frame(maxWidth: .infinity, alignment: .leading)
            
            // VSTACK #2 - HSTACK(INPUT PICKER + ARROW BUTTON) + GRID (START / END? PICKER ROWS) + TEXTFIELD? ======================
            VStack(alignment: .leading, spacing: 5.0) {
                
                // HSTACK #1 - INPUT TYPE PICKER + ARROW BUTTON ====================================================================
                HStack {
                    InputPicker(inputType: $inputType)
                        .frame(maxWidth: .infinity, alignment: .leading)
                        .onChange(of: inputType) {
                            searchItem = ""
                            if (inputType == InputOptions.iss || inputType == InputOptions.hubble) {
                                // Call directly the function to get the groundtrack data using either ISS or HUBBLE as input
                                Task {
                                    endTime = (showEndTime) ? endTime : startTime.addingTimeInterval(1.0)
                                    if (endTime > startTime) {
                                        await gtViewModel.fetchGroundTrack(inputType: inputType, searchItem: searchItem, start: startTime, end: endTime, step: step)
                                    }
                                }
                                mapCamera = .camera(
                                    MapCamera(
                                        centerCoordinate: CLLocationCoordinate2D(latitude: 0, longitude: 0),
                                        distance: 200_000_000,
                                        heading: 0.0,
                                        pitch: 0.0))
                                showDetails.toggle()
                                withAnimation(.easeInOut(duration: 0.3)) {
                                    showTextfield = false
                                }
                            } else {
                                withAnimation(.easeInOut(duration: 0.3)) {
                                    showTextfield = true
                                    gtViewModel.points = [:]
                                }
                            }
                        }
                    
                    Button("", systemImage: "arrow.right") {
                        // Call the function to get the groundtrack data
                        Task {
                            endTime = (showEndTime) ? endTime : startTime.addingTimeInterval(1.0)
                            if (endTime > startTime) {
                                await gtViewModel.fetchGroundTrack(inputType: inputType, searchItem: searchItem, start: startTime, end: endTime, step: step)
                            }
                        }
                        mapCamera = .camera(
                            MapCamera(
                                centerCoordinate: CLLocationCoordinate2D(latitude: 0, longitude: 0),
                                distance: 200_000_000,
                                heading: 0.0,
                                pitch: 0.0))
                        showDetails.toggle()
                    }
                    .tint(Color.burntSienna)
                }
                
                if (showTextfield) {
                    TextField(String("Search by \(inputType.rawValue.uppercased())..."), text: $searchItem)
                        .frame(maxWidth: .infinity, alignment: .leading)
                        .padding(.leading, 7)
                        .font(.textfieldFont)
                }
                
                // GRID #1 - START TEXT + DATE + TRACK TOGGLE (optional END TEXT + DATE + STEP) =====================================
                Grid(alignment: .leading, horizontalSpacing: 8){
                    
                    // GRID ROW for start date & time selection =====================================================================
                    GridRow(alignment: .center) {
                        
                        // Label for start picker
                        Text("Start")
                            .font(Font.startEndFont).kerning(2)
                            .gridColumnAlignment(.trailing)
                        
                        // Start date and time picker
                        DatePicker("", selection: $startTime, displayedComponents: [.date, .hourAndMinute])
                            .datePickerStyle(.compact)
                            .gridColumnAlignment(.center)
                            .fixedSize()
                            .font(Font.startEndFont)
                            .onChange(of: startTime) {
                                Task {
                                    endTime = (showEndTime) ? endTime : startTime.addingTimeInterval(1.0)
                                    if (endTime > startTime) {
                                        await gtViewModel.fetchGroundTrack(inputType: inputType, searchItem: searchItem, start: startTime, end: endTime, step: step)
                                    }
                                }
                                mapCamera = .camera(
                                    MapCamera(
                                        centerCoordinate: CLLocationCoordinate2D(latitude: 0, longitude: 0),
                                        distance: 200_000_000,
                                        heading: 0.0,
                                        pitch: 0.0))
                                showDetails.toggle()
                            }
                        
                        // Toggle for end time picker
                        RoundedRectangle(cornerRadius: 5)
                            .fill(showEndTime ? Color.gray.opacity(0.2) : Color.clear)
                            .overlay(
                                RoundedRectangle(cornerRadius: 5)
                                    .stroke(showEndTime ? Color.gray : Color.gray.opacity(0.4), lineWidth: 1)
                                .overlay(Text("Tracks?")
                                    .foregroundStyle(Color.gray)
                                    .font(Font.tracksStep)
                                    .padding(8))
                                    )
                            .frame(width: 75, height: 25)
                            .onTapGesture {
                                withAnimation(.easeInOut(duration: 0.3)) {
                                    showEndTime.toggle()
                                    // one sec difference from start to be able to kick off propagator
                                    endTime = (showEndTime) ? startTime.addingTimeInterval(60.0) : startTime.addingTimeInterval(1.0)
                                    Task {
                                        if (endTime > startTime) {
                                            await gtViewModel.fetchGroundTrack(inputType: inputType, searchItem: searchItem, start: startTime, end: endTime, step: step)
                                        }
                                    }
                                    mapCamera = .camera(
                                        MapCamera(
                                            centerCoordinate: CLLocationCoordinate2D(latitude: 0, longitude: 0),
                                            distance: 200_000_000,
                                            heading: 0.0,
                                            pitch: 0.0))
                                    showDetails.toggle()
                                }
                            }
        
                    }
                    .padding(.top, -10.0)
                    
                    
                    // GRID ROW in case "tracks" option is selected ==================================================================
                    if showEndTime {
                        GridRow(alignment: .center) {
                            
                            // Label for end picker
                            Text("End")
                                .font(Font.startEndFont).kerning(4).gridColumnAlignment(.trailing)
                            
                            // End date and time picker
                            DatePicker("", selection: $endTime, displayedComponents: [.date, .hourAndMinute])
                                .datePickerStyle(.compact)
                                .fixedSize()
                                .font(Font.inputButtonsFont)
                                .onChange(of: endTime) {
                                    Task {
                                        if (endTime > startTime) {
                                            await gtViewModel.fetchGroundTrack(inputType: inputType, searchItem: searchItem, start: startTime, end: endTime, step: step)
                                        }
                                    }
                                    mapCamera = .camera(
                                        MapCamera(
                                            centerCoordinate: CLLocationCoordinate2D(latitude: 0, longitude: 0),
                                            distance: 200_000_000,
                                            heading: 0.0,
                                            pitch: 0.0))
                                    showDetails.toggle()
                                }
                            
                            // Step selection (interval between two computations)
                            RoundedRectangle(cornerRadius: 5)
                                .fill(Color.clear)
                                .overlay(
                                    RoundedRectangle(cornerRadius: 5)
                                    .stroke(Color.gray, lineWidth: 1)
                                    .overlay(TextField("Step [m]", text: $step).keyboardType(.numberPad)
                                        .foregroundStyle(Color.gray)
                                        .font(Font.tracksStep)
                                        .padding(8))
                                        )
                                .frame(width: 75, height: 25)
                        }
                    }
                    
                }
                .padding(.top, 10)
                .padding(.bottom, 10)
                
            }
        
            
            // View of the 2D Map
            Map (initialPosition: mapCamera) {
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
            .clipShape(RoundedRectangle(cornerRadius: 20))
            .frame(maxHeight: 300)
            .mapStyle(.standard(elevation: .flat))
                
            if(showDetails) {
                VStack{
                    Text(gtViewModel.satName)
                }
                .frame(maxWidth: .infinity, minHeight: 150.0)
                .clipShape(RoundedRectangle(cornerRadius: 20, style: .continuous))
                .overlay(
                    RoundedRectangle(cornerRadius: 20, style: .continuous)
                        .stroke(Color.forestGreen, lineWidth: 1)
                )
            }
            
                
        }
        .padding(.top, 60)
        .padding([.leading, .trailing], 30)
        .ignoresSafeArea()
        
        Spacer()
    }
}
   

#Preview {
    Homepage()
}
