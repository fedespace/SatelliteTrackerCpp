//
//  FontTypes.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 20/05/2026.
//

import SwiftUI

extension Font {
    
    // Eunomia fonts
    static let eunomiaLight = Font.custom("Eunomia-Light", size: 24)
    static let eunomiaRegular = Font.custom("Eunomia-Regular", size: 24)
    static let eunomiaBold = Font.custom("Eunomia-Bold", size: 24)
    
    // Main title of the page
    static let mainTitleFont = Font.custom("Eunomia-Regular", size: 55)
    
    // Custom input picker
    static let inputPickerTitle = Font.custom("Electrolize", size: 15)
    static let inputPickerFont = Font.custom("Electrolize", size: 18)
    static let inputPickerDropdown = Font.custom("Electrolize", size: 13)
    static let inputFavs = Font.custom("Electrolize", size: 16)

    
    // Step / groundtrack buttons
    static let inputButtonsFont = Font.custom("Electrolize", size: 15)

    // Start and End textfield for datepicker
    static let startEndFont = Font.custom("Rajdhani-Medium", size: 18)
    
    // Hint textfield
    static let textfieldFont = Font.custom("Rajdhani-Regular", size: 16)
    
    // Tracks / step hints
    static let tracksStep = Font.custom("Rajdhani-Regular", size: 16)
    
    // Details Title
    static let detailTitle = Font.custom("Eunomia-Regular", size: 30)
    
    // Details body
    static let detailBody = Font.custom("Electrolize", size: 20)
    
    // Subtitle details body
    static let subtitleDetail = Font.custom("Electrolize", size: 22)
    
    // GS
    static let gsTitle = Font.custom("Rajdhani-Regular", size: 25)
    static let gsWindow = Font.custom("Rajdhani-Regular", size: 20)
    static let coord = Font.custom("Rajdhani-Regular", size: 18)
    static let pass = Font.custom("Electrolize", size: 20)
    static let fetchPass = Font.custom("Electrolize", size: 17)
}

