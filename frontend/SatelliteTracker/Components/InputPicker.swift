//
//  InputPicker.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 20/05/2026.
//

import SwiftUI

// Picker options for user input selection
enum InputOptions: String, CaseIterable {
    case name
    case norad
    case tle
}

struct InputPicker: View {
    
    @Binding var inputType: InputOptions
    @Binding var searchItem: String?
    @Binding var detailsSatellite: SatelliteDetails
    @Binding var endTime: Date
    @Binding var startTime: Date
    @Binding var showDetails: Bool
    @Binding var showEndTime: Bool
    @Binding var gtViewModel: GroundTrackViewModel
    @Binding var step: String
    @State private var defaultInputArray = InputOptions.allCases.prefix(3)
    @State private var savedInputs = Array(InputOptions.allCases.dropFirst(3))
    @State private var savedMenuExpanded: Bool = false
    var saved: Bool {
        // fill the heart if what I wrote in the textfield matches one of the entries in the favs
        if inputType == InputOptions.name {
            let alreadyFavs = Set(favouritesStore.savedInputs.map{$0.name})
            return alreadyFavs.contains(searchItem)
        } else if inputType == InputOptions.norad {
            let alreadyFavs = Set(favouritesStore.savedInputs.map{$0.norad})
            return alreadyFavs.contains(searchItem)
        }
        return false
    }
    @ObservedObject var favouritesStore: FavouritesStore
    
    
    var body: some View {
        
        HStack {
            
            GlassEffectContainer {
                HStack (alignment: .center) {
                    
                    ForEach(defaultInputArray, id: \.self) { option in
                        Text(option.rawValue.uppercased())
                            .onTapGesture {
                                inputType = option                            }
                            .animation(.easeInOut(duration: 0.3), value: inputType)
                            .foregroundStyle(
                                (inputType == option) ? Color.black : Color.black.opacity(0.2)
                            )
                            .font(Font.inputPickerFont)
                        
                        if option != defaultInputArray.last {
                            Text("•")
                                .foregroundStyle(
                                    Color.black.opacity(0.2)
                                )
                        }
                    }
                }
                .padding([.top, .bottom], 2)
                .padding([.leading, .trailing], 10)
                .glassEffect(.regular.tint(.white.opacity(0.6)))
            }
            
            Spacer()
            
            GlassEffectContainer(spacing: 20) {
                HStack(spacing: 40.0) {
                    
                    ZStack {
                        
                        
                        ForEach(Array("FAVS".enumerated()), id: \.offset) { index, letter in
                            let initialDeg = savedMenuExpanded ? 0.0 : -45.0
                            let angle = angleFor(index: index, total: "FAVS".count, initialDeg: initialDeg)
                            Text(String(letter))
                                .font(.inputFavs)
                                .foregroundStyle(Color.ivoryMist)
                                .offset(y: -20)
                                .rotationEffect(angle)
                        }
                       
                           
                        
                        Button {
                            withAnimation(.easeInOut(duration: 0.5)) {
                                savedMenuExpanded.toggle()
                            }
                        } label: {
                            Image(systemName: "arrowtriangle.down")
                                .scaleEffect(x: 1.0, y: 0.8)
                                .font(.system(size: 16))
                                .foregroundStyle(Color.black)
                                .rotationEffect(.degrees(savedMenuExpanded ? 90 : 0))
                        }
                        .frame(width: 25, height:  25)
                        .glassEffect()
                        .overlay {
                            if savedMenuExpanded {
                                VStack (alignment: .leading, spacing: 0) {
                                    ForEach(favouritesStore.savedInputs) { satellite in
                                        Text(
                                            (satellite.name != nil) ? satellite.name! : (satellite.norad != nil) ? satellite.norad! : ""
                                        )
                                        .onTapGesture {
                                            if satellite.name != nil {
                                                inputType = InputOptions.name
                                                searchItem = satellite.name!
                                                Task {
                                                    detailsSatellite = SatelliteDetails.empty
                                                    endTime = (showEndTime) ? endTime : startTime.addingTimeInterval(1.0)
                                                    if (endTime > startTime) {
                                                        await gtViewModel.fetchGroundTrack(inputType: inputType, searchItem: searchItem!.uppercased(), start: startTime, end: endTime, step: step)
                                                    }
                                                }
                                                showDetails = true
                                            } else if satellite.norad != nil {
                                                inputType = InputOptions.norad
                                                searchItem = satellite.norad!
                                            }
                                            withAnimation(.easeIn(duration: 0.4)) {
                                                savedMenuExpanded.toggle()
                                            }
                                        }
                                        .font(.inputPickerFont)
                                        .foregroundStyle(.black)
                                    }
                                }
                                .padding(.vertical, 5)
                                .padding(.horizontal,10)
                                .frame(width: 160, alignment: .leading)
                                .multilineTextAlignment(.leading)
                                .glassEffect(in: .rect(cornerRadius: 18))
                                .offset(x: -100, y: 15)
                            }
                        }
                    }
                    
                    
                           
                    Button {
                        withAnimation(.easeInOut(duration: 0.5)) {
                            if saved {
                                favouritesStore.remove(searchItem: searchItem!, input: inputType)
                            }
                            else {
                                if (inputType == InputOptions.name) {
                                    let alreadyFavs = Set(favouritesStore.savedInputs.map {$0.name})
                                    if !alreadyFavs.contains(searchItem) {
                                        favouritesStore.add(SavedSatellites(id: UUID(), name: searchItem, norad: nil))
                                    }
                                } else if (inputType == InputOptions.name) {
                                    let alreadyFavs = Set(favouritesStore.savedInputs.map {$0.norad})
                                    if !alreadyFavs.contains(searchItem) {
                                        favouritesStore.add(SavedSatellites(id: UUID(), name: nil, norad: searchItem))
                                    }
                                }
                            }
                        }
                    } label: {
                        Image(systemName: saved ? "heart.fill" : "heart")
                            .scaleEffect(x: 1.0, y: 0.9)
                            .font(.system(size: 16))
                            .foregroundStyle(saved ? Color.red.opacity(0.6) : inputType == InputOptions.tle ? Color.gray : Color.black)
                            .symbolEffect(.bounce, value: saved)
                        
                            
                    }
                    .frame(width: 25, height:  25)
                    .glassEffect()


                        // An `offset` shows how Liquid Glass effects react to each other in a container.
                        // Use animations and components appearing and disappearing to obtain effects that look purposeful.
                        .offset(x: -36.0, y: 0.0)
                }
            }
            
            
            
        }
        .padding(.trailing, -25)
        
        
    }
    
    private func angleFor(index: Int, total: Int, initialDeg: Double) -> Angle {
        let step = 27.0 // can be modified
        let start = -Double(total - 1) * step / 2
        return .degrees(initialDeg + start + Double(index) * step)
    }
    
}
