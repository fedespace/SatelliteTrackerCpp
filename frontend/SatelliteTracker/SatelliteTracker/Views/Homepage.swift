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
    
    var body: some View {
        
        VStack(alignment: .center, spacing: 10) {
            
            // Title of the Main Home Page
            Text("SkyLens")
                .font(.mainTitleFont)
                .kerning(7)
                .frame(maxWidth: .infinity, alignment: .leading)
            
            // Creating the grid to add inputs
            Grid {
                GridRow {
                    InputPicker(inputType: $inputType)
                    
                }
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
