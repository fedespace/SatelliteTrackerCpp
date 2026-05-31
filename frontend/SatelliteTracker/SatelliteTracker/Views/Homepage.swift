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
    @State private var mapCamera: MapCameraPosition = .camera(
        MapCamera(
            centerCoordinate: CLLocationCoordinate2D(latitude: 0, longitude: 0),
            distance: 200_000_000,
            heading: 0.0,
            pitch: 0.0))
    @State private var startTime = Date()
    @State private var endTime = Date()
    @State private var showEndTime = false
    @State private var step: String = ""
    
    var body: some View {
        
        VStack(spacing: 10) {
            
            // Title of the Main Home Page
            Text("SkyLens")
                .font(.mainTitleFont)
                .kerning(7)
                .frame(maxWidth: .infinity, alignment: .leading)
            
            // Creating the VSTACK to add inputs
            VStack(alignment: .leading, spacing: 5) {
                
                // input picker
                InputPicker(inputType: $inputType)
                    .frame(maxWidth: .infinity, alignment: .leading)
                    .onChange(of: inputType) {
                        searchItem = ""
                    }
                
                // Pick dates and toggle for single point or groundtrack
                Grid(alignment: .leading, horizontalSpacing: 8){
                    GridRow(alignment: .center) {
                        
                        // Label for start picker
                        Text("Start")
                        
                        // Start picker
                        DatePicker("", selection: $startTime, displayedComponents: [.date, .hourAndMinute])
                            .datePickerStyle(.compact)
                            .fixedSize()
                        
                        // Toggle for end time
                        RoundedRectangle(cornerRadius: 5)
                            .fill(showEndTime ? Color.gray.opacity(0.2) : Color.clear)
                            .overlay(
                                RoundedRectangle(cornerRadius: 5)
                                    .stroke(showEndTime ? Color.gray : Color.gray.opacity(0.4), lineWidth: 1)
                                .overlay(Text("Tracks?")
                                    .foregroundStyle(Color.gray)
                                    .font(Font.inputButtonsFont)
                                    .padding(8))
                                    )
                            .frame(width: 75, height: 25)
                            .onTapGesture {
                                withAnimation(.easeInOut(duration: 0.3)) {
                                        showEndTime.toggle()
                                    }
                            }
        
                    }
                    
                    if showEndTime {
                        GridRow(alignment: .center) {
                            // Label for start picker
                            Text("End")
                            
                            // Start picker
                            DatePicker("", selection: $endTime, displayedComponents: [.date, .hourAndMinute])
                                .datePickerStyle(.compact)
                                .fixedSize()
                            
                            // Step selection
                            RoundedRectangle(cornerRadius: 5)
                                .fill(Color.clear)
                                .overlay(
                                    RoundedRectangle(cornerRadius: 5)
                                    .stroke(Color.gray, lineWidth: 1)
                                    .overlay(TextField("Step [m]", text: $step).keyboardType(.numberPad)
                                        .foregroundStyle(Color.gray)
                                        .font(Font.inputButtonsFont)
                                        .padding(8))
                                        )
                                .frame(width: 75, height: 25)
                        }
                    }
                    
                }
                .padding(.top, 10)
                .padding(.bottom, 10)
                    
                
                // HSTACK with textfield & confirm arrow
                HStack {
                    TextField(String("Search by \(inputType.rawValue.uppercased())..."), text: $searchItem)

                    Button("", systemImage: "arrow.right") {
                        // Call the function to get the groundtrack data
                        Task {
                            if (endTime > startTime) {
                                await gtViewModel.fetchGroundTrack(inputType: inputType, searchItem: searchItem, start: startTime, end: endTime, step: step)
                            }
                        }
                    }
                    .tint(Color.burntSienna)
                }
                .frame(maxWidth: .infinity, alignment: .leading)
                .padding(.leading, 7)
                
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
            .frame(maxWidth: .infinity, maxHeight: 350)
            .mapStyle(.standard(elevation: .flat))
                
    
                
                
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
