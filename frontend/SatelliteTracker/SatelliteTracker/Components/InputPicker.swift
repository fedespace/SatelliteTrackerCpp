//
//  InputPicker.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 20/05/2026.
//

import SwiftUI

// Picker options for user input selection
enum InputOptions: String, CaseIterable {
    case tle
    case name
    case norad
    case iss
    case hubble
}

struct InputPicker: View {
    
    @Binding var inputType: InputOptions
    
    var body: some View {
        
        HStack {
            ForEach(InputOptions.allCases, id: \.self) { option in
                Text(option.rawValue.uppercased())
                    .padding(5)
                    .background(
                        inputType == option ? Color.gray.opacity(0.7) : nil
                    )
                    .animation(.easeInOut(duration: 0.3), value: inputType)
                    .foregroundStyle(
                        inputType == option ? Color.white : Color.black
                    )
                    .clipShape(.rect(cornerRadius: 50.0))
                    .font(
                        inputType == option ? Font.inputPickerFont.bold() : Font.inputPickerFont)
                    .onTapGesture {
                        inputType = option
                    }
                
                
            }
        }
        .frame(maxWidth: .infinity)
        .padding([.top, .bottom], 5)
        .background(Color.gray.opacity(0.15))
        .clipShape(.capsule)
    }
}
