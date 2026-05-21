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
                    
                    Button("", systemImage: "arrow.right") {
                        // action
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
