//
//  Homepage.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 20/05/2026.
//

import SwiftUI

// Homepage structure
struct Homepage: View {
    
    @State private var inputType: InputOptions = .tle
    @State private var searchItem: String = ""
    @State private var gtViewModel = GroundTrackViewModel()
        
    var body: some View {
        
        VStack(spacing: 10) {
            
            // Title of the Main Home Page
            Text("SkyLens")
                .font(.mainTitleFont)
                .kerning(7)
                .frame(maxWidth: .infinity, alignment: .leading)
            
            // Creating the VSTACK to add inputs
            VStack(spacing: 5) {
                
                // input picker
                InputPicker(inputType: $inputType)
                    .frame(maxWidth: .infinity, alignment: .leading)
                
                // HSTACK with textfield & confirm arrow
                HStack {
                    TextField(String("Search by \(inputType.rawValue.uppercased())..."), text: $searchItem)
                    
                    let _ = print("view loaded")

                    Button("", systemImage: "arrow.right") {
                        // Call the function to get the groundtrack data
                        Task {
                            await gtViewModel.fetchGroundTrack(tle: TleRequest(
                                name: "ONEWEB-0162",
                                line1: "1 48056U 21025Q   26046.24572298 -.00000136  00000+0 -40788-3 0  9991",
                                line2: "2 48056  87.9044 344.4620 0001950  75.0816 285.0529 13.13470762237537"
                            ))
                        }
                    }
                    .tint(Color.burntSienna)
                }
                .frame(maxWidth: .infinity, alignment: .leading)
                .padding(.leading, 7)
            }
            Spacer()
            
            Text("Other content")
            
        }
        .padding(.top, 60)
        .padding([.leading, .trailing], 30)
        .ignoresSafeArea()
    }
}
   

#Preview {
    Homepage()
}
